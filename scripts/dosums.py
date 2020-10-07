import hashlib
import sys
if len(sys.argv) <= 1:
    print("Needs args")
    exit(1)
filename = sys.argv[1]
print(filename)
with open(filename, "rb") as f:
    byteBuf = f.read()
    with open(filename + ".md5", "w") as wf:
        wf.write(hashlib.md5(byteBuf).hexdigest())
    with open(filename + ".sha1", "w") as wf:
        wf.write(hashlib.sha1(byteBuf).hexdigest())
    with open(filename + ".sha256", "w") as wf:
        wf.write(hashlib.sha256(byteBuf).hexdigest())
    with open(filename + ".sha512", "w") as wf:
        wf.write(hashlib.sha512(byteBuf).hexdigest())
