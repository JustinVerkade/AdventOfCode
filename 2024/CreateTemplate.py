import requests
import os

def main():
    # load environment cookies
    with open(".env", "r") as env:
        for line in env.readlines():
            line = line.replace("\n", "")
            tag, value = line.split("=")
            os.environ[tag] = str(value)

    # get session cookie
    session_key = os.getenv("AOC_SESSION_KEY")
    if session_key == None:
        print("Please export AoC session key!")
        return

    # copy template structure
    day = int(input("Enter day:"))
    os.system(f"cp -r ./2024/Template ./2024/Day_{day:02d}")

    # write input to data.txt
    with open(f"./2024/Day_{day:02d}/build/data.txt", "w") as file:
        cookies = {"session": session_key}
        get_request = requests.get(f"https://adventofcode.com/2024/day/{day}/input", cookies=cookies)
        raw_data = get_request.text
        file.write(raw_data)

if __name__ == "__main__":
    main()