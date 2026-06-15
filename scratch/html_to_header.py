#!/usr/bin/env python3
import os

def main():
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    html_path = os.path.join(root_dir, "web-tool.html")
    header_path = os.path.join(root_dir, "include", "html_index.h")

    if not os.path.exists(html_path):
        print(f"Error: {html_path} not found.")
        return

    print(f"Reading {html_path}...")
    with open(html_path, "r", encoding="utf-8") as f:
        html = f.read()

    print(f"Writing {header_path}...")
    header_content = f"""#ifndef HTML_INDEX_H
#define HTML_INDEX_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
{html}
)rawliteral";

#endif // HTML_INDEX_H
"""

    with open(header_path, "w", encoding="utf-8") as f:
        f.write(header_content)
    
    print("Regeneration complete!")

if __name__ == "__main__":
    main()
