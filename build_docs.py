import os
import subprocess
import sys

def install_dependencies():
    """Install MkDocs and the necessary plugins."""
    try:
        # Check if MkDocs is installed
        subprocess.run([sys.executable, "-m", "pip", "install", "mkdocs"], check=True)
        subprocess.run([sys.executable, "-m", "pip", "install", "mkdocs-material"], check=True)
        print("Successfully installed MkDocs and MkDocs Material.")
    except subprocess.CalledProcessError as e:
        print("Error installing dependencies:", e)
        sys.exit(1)

def build_docs():
    """Build the documentation using MkDocs."""
    try:
        subprocess.run(["mkdocs", "build"], check=True)
        print("Documentation built successfully.")
    except subprocess.CalledProcessError as e:
        print("Error building documentation:", e)
        sys.exit(1)

def main():
    print("Starting the documentation build process...")
    install_dependencies()
    build_docs()
    print("Documentation build process completed.")

if __name__ == "__main__":
    main()
