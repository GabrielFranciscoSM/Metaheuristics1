import csv
import os
import glob
import math

#-------------------------------------------------
# Configuration
#-------------------------------------------------
# Directory where your result CSV files are located
RESULTS_DIR = './' # Current directory/results subdir
# Pattern to match result files
FILE_PATTERN = 'output*.txt'

#-------------------------------------------------
# Helper function for number formatting (same as before)
#-------------------------------------------------
def format_num(num, precision=2, sci_thresh_low=1e-3, sci_thresh_high=1e4):
    """Formats numbers for the table."""
    if num is None: return 'N/A' # Handle potential missing data
    try:
        num = float(num) # Ensure it's numeric
    except (ValueError, TypeError):
        return str(num) # Return as string if not convertible

    if num == 0:
        return f"{0:.{precision}f}"
    abs_num = abs(num)
    if abs_num > 0 and (abs_num < sci_thresh_low or abs_num >= sci_thresh_high):
        return f"{num:.{precision}e}"
    else:
        return f"{num:.{precision}f}"

#-------------------------------------------------
# Function to read data from a single CSV file
#-------------------------------------------------
def read_data_from_file(filepath):
    """Reads structured data from a CSV file."""
    data = []
    try:
        with open(filepath, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            expected_headers = ['name', 'bestFitness', 'meanFit', 'meanTime', 'meanEvals']
            if not all(h in reader.fieldnames for h in expected_headers):
                 print(f"Warning: File '{filepath}' has missing headers. Expected: {expected_headers}. Found: {reader.fieldnames}. Skipping file.")
                 return []

            for row in reader:
                try:
                    # Convert numeric types
                    row_data = {
                        'name': row.get('name', 'Unknown'), # Use .get for safety
                        'bestFitness': float(row['bestFitness']),
                        'meanFit': float(row['meanFit']),
                        'meanTime': float(row['meanTime']),
                        'meanEvals': int(float(row['meanEvals'])), # Evals usually integer
                        # Add other fields if they exist, handle potential errors
                    }
                    data.append(row_data)
                except (ValueError, KeyError) as e:
                    print(f"Warning: Skipping row in '{filepath}' due to data error: {e}. Row: {row}")
                    continue # Skip malformed rows
    except FileNotFoundError:
        print(f"Error: File not found: {filepath}")
        return []
    except Exception as e:
        print(f"Error reading file {filepath}: {e}")
        return []
    return data

#-------------------------------------------------
# Function to generate LaTeX table code (modified slightly)
#-------------------------------------------------
def generate_latex_table(data, caption="Performance Results", label="tab:results",
                         sort_by=None, reverse_sort=False,
                         columns=None, column_formats=None,
                         precision=2, sci_thresh_low=1e-3, sci_thresh_high=1e4):
    """
    Generates LaTeX code for a table from the structured data.
    Requires LaTeX packages: booktabs
    """
    if not data:
        return "% No data provided or read\n"

    # --- Default Columns ---
    # If columns aren't specified, try to determine from data, including 'run_id'
    if columns is None:
        if data:
            columns = list(data[0].keys())
            # Try to put 'run_id' and 'name' first if they exist
            default_order = ['run_id', 'name']
            remaining_cols = [c for c in columns if c not in default_order]
            columns = [c for c in default_order if c in columns] + sorted(remaining_cols)
        else:
            columns = [] # No data, no columns

    # --- Sorting ---
    if sort_by:
        if sort_by not in columns:
            print(f"Warning: Sort key '{sort_by}' not found in data keys. Sorting ignored.")
        else:
            try:
                # Use .get with a default that won't break sorting (e.g., 0 or infinity)
                default_sort_val = float('inf') if not reverse_sort else float('-inf')
                data.sort(key=lambda x: x.get(sort_by, default_sort_val), reverse=reverse_sort)
            except TypeError:
                 print(f"Warning: Could not sort by '{sort_by}' due to incompatible data types. Sorting ignored.")


    # --- LaTeX Preamble & Table Structure ---
    col_align = []
    for col in columns:
        # Basic heuristic: 'name' and 'run_id' left, others right
        if col in ['name', 'run_id']:
            col_align.append('l')
        else:
            col_align.append('r')

    # Generate only the table environment, not the full document
    # User should wrap this in \documentclass, \usepackage{booktabs}, \begin{document} etc.
    latex_str = f"% --- Generated LaTeX Table: {label} ---\n"
    latex_str += "\\begin{table}[htbp] % Use 'H' from float package for 'exactly here'\n"
    latex_str += "  \\centering\n"
    latex_str += f"  \\caption{{{caption}}}\n"
    latex_str += f"  \\label{{{label}}}\n"
    # Make table potentially smaller if many columns
    latex_str += "  \\resizebox{\\textwidth}{!}{%\n" # Scale table to text width if needed
    latex_str += f"  \\begin{{tabular}}{{ {' '.join(col_align)} }}\n"
    latex_str += "    \\toprule\n"

    # --- Header Row ---
    header_names = { # Nicer names for headers
        'run_id': 'Run ID',
        'name': 'Name',
        'bestFitness': 'Best Fitness',
        'meanFit': 'Mean Fitness',
        'meanTime': 'Mean Time (s)',
        'meanEvals': 'Mean Evals'
    }
    header = [f"\\textbf{{{header_names.get(col, col.replace('_', ' ').title())}}}" for col in columns]
    latex_str += "    " + " & ".join(header) + " \\\\\n"
    latex_str += "    \\midrule\n"

    # --- Data Rows ---
    for row_data in data:
        row_values = []
        for col in columns:
            value = row_data.get(col, 'N/A') # Use .get for safety

            # Apply custom format if specified
            if column_formats and col in column_formats:
                 formatted_value = column_formats[col](value) if callable(column_formats[col]) else column_formats[col] % value
            # Default formatting
            elif isinstance(value, (int, float)):
                formatted_value = format_num(value, precision, sci_thresh_low, sci_thresh_high)
            elif isinstance(value, str):
                 # Escape special LaTeX characters in strings
                 formatted_value = value.replace('\\', '\\textbackslash{}') # Must be first
                 formatted_value = formatted_value.replace('_', '\\_')
                 formatted_value = formatted_value.replace('%', '\\%')
                 formatted_value = formatted_value.replace('&', '\\&')
                 formatted_value = formatted_value.replace('#', '\\#')
                 formatted_value = formatted_value.replace('$', '\\$')
                 formatted_value = formatted_value.replace('{', '\\{')
                 formatted_value = formatted_value.replace('}', '\\}')
                 formatted_value = formatted_value.replace('~', '\\textasciitilde{}')
                 formatted_value = formatted_value.replace('^', '\\textasciicircum{}')

            else:
                formatted_value = str(value) # Fallback

            row_values.append(formatted_value)

        latex_str += "    " + " & ".join(row_values) + " \\\\\n"

    # --- Footer ---
    latex_str += "    \\bottomrule\n"
    latex_str += "  \\end{tabular}%\n"
    latex_str += "  } % End resizebox\n" # Add closing brace for resizebox
    latex_str += "\\end{table}\n"
    latex_str += "% --- End Generated Table ---\n"
    # Note about missing std dev is still relevant
    latex_str += "% Note: Mean values are averages over runs *within* the original experiment setup.\n"
    latex_str += "% Standard deviations for these means were not available in the input files.\n"

    return latex_str

#-------------------------------------------------
# Main Script Logic
#-------------------------------------------------
all_data = []
search_path = os.path.join(RESULTS_DIR, FILE_PATTERN)
file_list = glob.glob(search_path)

if not file_list:
    print(f"No files found matching '{search_path}'")
else:
    print(f"Found {len(file_list)} files to process:")
    for filepath in sorted(file_list): # Sort files for consistent processing order
        print(f"  - Reading {filepath}")
        # Extract a Run ID from the filename
        filename = os.path.basename(filepath)
        run_id, _ = os.path.splitext(filename) # Get filename without extension
        # You might want to clean up the run_id further, e.g., remove prefixes
        # run_id = run_id.replace('results_', '').replace('_final', '')

        file_data = read_data_from_file(filepath)

        # Add the run_id to each record from this file
        for record in file_data:
            record['run_id'] = run_id # Add the identifier

        all_data.extend(file_data)

print(f"\nTotal records processed: {len(all_data)}")

#-------------------------------------------------
# Generate LaTeX Output
#-------------------------------------------------
if all_data:
    # Example 1: Table sorted by Mean Fitness (descending) across all runs
    latex_code_sorted_fitness = generate_latex_table(
        all_data, # Use the combined data
        caption="Combined Performance Results Sorted by Mean Fitness (Higher is Better)",
        label="tab:combined_sorted_fit",
        sort_by='meanFit',
        reverse_sort=True, # Higher fitness is better
        precision=3
    )
    print("\n--- LaTeX Table: Combined Data Sorted by Mean Fitness ---")
    print(latex_code_sorted_fitness)

    # Example 2: Table sorted by Mean Time (ascending) across all runs
    latex_code_sorted_time = generate_latex_table(
        all_data, # Use the combined data
        caption="Combined Performance Results Sorted by Mean Time (Lower is Better)",
        label="tab:combined_sorted_time",
        sort_by='meanTime',
        reverse_sort=False, # Lower time is better
        precision=1
    )
    # print("\n--- LaTeX Table: Combined Data Sorted by Mean Time ---")
    # print(latex_code_sorted_time)

    # Example 3: Table sorted by Run ID (alphabetical), then maybe Name
    # To sort by multiple keys, sort the data beforehand
    # all_data.sort(key=lambda x: (x.get('run_id', ''), x.get('name', '')))
    # latex_code_sorted_runid = generate_latex_table(
    #     all_data,
    #     caption="Combined Performance Results Sorted by Run ID",
    #     label="tab:combined_sorted_runid",
    #     sort_by=None # Already sorted
    # )
    # print("\n--- LaTeX Table: Combined Data Sorted by Run ID ---")
    # print(latex_code_sorted_runid)

    # --- Saving to a file ---
    # You would typically generate one or more tables and save them.
    output_filename = "combined_results_table.tex"
    try:
        with open(output_filename, "w") as f:
            # Add necessary LaTeX document preamble IF this is the ONLY content
            # If inserting into an existing document, just write the table code.
            f.write("\\documentclass{article}\n")
            f.write("\\usepackage{booktabs} % For nice table rules\n")
            f.write("\\usepackage{graphicx} % For resizebox\n")
            f.write("\\usepackage[margin=1in]{geometry} % Adjust margins if needed\n")
            f.write("\\usepackage{amsmath}\n\n")
            f.write("\\begin{document}\n\n")

            # Write the desired table(s)
            f.write(latex_code_sorted_fitness)
            # f.write("\n\\newpage\n\n") # Optional page break between tables
            # f.write(latex_code_sorted_time)

            f.write("\n\\end{document}\n")
        print(f"\nLaTeX code saved to '{output_filename}'")
        print(f"Compile using: pdflatex {output_filename}")
    except IOError as e:
        print(f"\nError writing LaTeX output to file: {e}")

else:
    print("\nNo data loaded, skipping LaTeX table generation.")