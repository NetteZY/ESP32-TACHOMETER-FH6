#!/usr/bin/env python3
import json
import urllib.request
import urllib.error
import os
import sys

def create_github_release():
    owner = "NetteZY"
    repo = "ESP32-TACHOMETER-FH6"
    tag = "v1.1.3"
    
    # Read token
    token_path = "/home/kuraaku/ESP32-TELE/token"
    if not os.path.exists(token_path):
        print(f"Error: Token file not found at {token_path}")
        sys.exit(1)
        
    with open(token_path, "r") as f:
        token = f.read().strip()
        
    if not token:
        print("Error: Token is empty")
        sys.exit(1)
 
    headers = {
        "Authorization": f"Bearer {token}",
        "Accept": "application/vnd.github+json",
        "X-GitHub-Api-Version": "2022-11-28",
        "User-Agent": "Python-urllib"
    }
 
    # Step 1: Check if release already exists and delete it
    print(f"Checking if release {tag} exists...")
    check_url = f"https://api.github.com/repos/{owner}/{repo}/releases/tags/{tag}"
    check_req = urllib.request.Request(check_url, headers=headers, method="GET")
    
    release_id = None
    try:
        with urllib.request.urlopen(check_req) as res:
            res_data = json.loads(res.read().decode("utf-8"))
            release_id = res_data["id"]
            print(f"Found existing release with ID: {release_id}. Deleting...")
    except urllib.error.HTTPError as e:
        if e.code == 404:
            print("No existing release found.")
        else:
            print(f"Error checking release: {e.code} - {e.reason}")
            
    if release_id:
        # Delete release
        delete_url = f"https://api.github.com/repos/{owner}/{repo}/releases/{release_id}"
        delete_req = urllib.request.Request(delete_url, headers=headers, method="DELETE")
        try:
            with urllib.request.urlopen(delete_req) as res:
                print("Release deleted successfully.")
        except Exception as e:
            print(f"Warning: Failed to delete release: {e}")
 
        # Delete tag ref
        delete_tag_url = f"https://api.github.com/repos/{owner}/{repo}/git/refs/tags/{tag}"
        delete_tag_req = urllib.request.Request(delete_tag_url, headers=headers, method="DELETE")
        try:
            with urllib.request.urlopen(delete_tag_req) as res:
                print("Tag reference deleted successfully.")
        except Exception as e:
            print(f"Warning: Failed to delete tag reference: {e}")
 
    # Step 2: Create a new release
    print(f"Creating new GitHub release {tag}...")
    create_url = f"https://api.github.com/repos/{owner}/{repo}/releases"
    data = {
        "tag_name": tag,
        "target_commitish": "main",
        "name": tag,
        "body": "Version 1.1.3 Release: Bug Fixes & Refinements\n\n### Changes:\n- **Web Tool Fixes**: Fixed `showTab` undefined reference and added diagnostic logging to the web configuration tool.\n- **PNA Restrictions**: Bypassed Chromium Private Network Access (PNA) restrictions for direct connections.\n- **Defensive Parsing**: Added defensive optional chaining to the JSON config parsing.\n\n### Included Assets:\n- `merged-firmware.bin`: Unified flash binary (burn at offset 0x0).\n- `firmware.bin`: App application binary (PlatformIO upload at offset 0x10000).\n- `bootloader.bin` & `partitions.bin`.",
        "draft": False,
        "prerelease": False
    }

    req = urllib.request.Request(
        create_url, 
        data=json.dumps(data).encode("utf-8"), 
        headers=headers, 
        method="POST"
    )

    try:
        with urllib.request.urlopen(req) as res:
            res_data = json.loads(res.read().decode("utf-8"))
            release_id = res_data["id"]
            upload_url_template = res_data["upload_url"]
            print(f"New release created! ID: {release_id}")
    except urllib.error.HTTPError as e:
        print(f"HTTP Error creating release: {e.code} - {e.reason}")
        print(e.read().decode("utf-8"))
        sys.exit(1)

    base_upload_url = upload_url_template.split("{")[0]

    # Files to upload
    files_to_upload = {
        "merged-firmware.bin": "/home/kuraaku/ESP32-TELE/merged-firmware.bin",
        "firmware.bin": "/home/kuraaku/ESP32-TELE/.pio/build/esp32dev/firmware.bin",
        "bootloader.bin": "/home/kuraaku/ESP32-TELE/.pio/build/esp32dev/bootloader.bin",
        "partitions.bin": "/home/kuraaku/ESP32-TELE/.pio/build/esp32dev/partitions.bin"
    }

    for filename, filepath in files_to_upload.items():
        if not os.path.exists(filepath):
            print(f"Warning: File {filepath} not found, skipping...")
            continue
            
        print(f"Uploading {filename} ({os.path.getsize(filepath)} bytes)...")
        upload_url = f"{base_upload_url}?name={filename}"
        
        with open(filepath, "rb") as f:
            file_bytes = f.read()

        upload_headers = headers.copy()
        upload_headers["Content-Type"] = "application/octet-stream"
        
        upload_req = urllib.request.Request(
            upload_url,
            data=file_bytes,
            headers=upload_headers,
            method="POST"
        )
        
        try:
            with urllib.request.urlopen(upload_req) as res:
                print(f"Successfully uploaded {filename}!")
        except Exception as e:
            print(f"Error uploading {filename}: {e}")

    print("\nRelease regeneration completed.")

if __name__ == "__main__":
    create_github_release()
