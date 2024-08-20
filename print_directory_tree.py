import os

def print_directory_tree(rootdir, indent=''):
	"""
	Print the directory tree with given root directory using custom characters.

	Args:
	- rootdir (str): Root directory whose tree structure is to be printed.
	- indent (str): Current level of indentation for printing.

	Returns:
	- None
	"""
	if not os.path.isdir(rootdir):
		print(f"{rootdir} is not a valid directory.")
		return
	
	files = os.listdir(rootdir)
	files.sort()  # Sorting files alphabetically

	for i, file in enumerate(files):
		fullpath = os.path.join(rootdir, file)
		is_last = (i == len(files) - 1)
		branch = '└── ' if is_last else '├── '
		print(f"{indent}{branch}{file}")

		if os.path.isdir(fullpath):
			print_directory_tree(fullpath, indent + ('	' if is_last else '│   '))

# Example usage:
if __name__ == "__main__":
	directory_path = "."  # Change this to the directory path you want to print
	print(f"Directory tree of '{directory_path}':")
	print_directory_tree(directory_path)
