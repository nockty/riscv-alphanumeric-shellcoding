import re
import subprocess


def process_url(url):
    args = "./vuln.bin"
    popen = subprocess.Popen(
        args,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        universal_newlines=True,
    )
    output_lines = popen.communicate(url)
    return output_lines


def is_url(data):
    return bool(re.match(r"^[\w\/\.]+$", data))
