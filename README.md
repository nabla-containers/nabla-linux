# Nabla Linux

# One liner

```
docker run --rm -it kollerr/linux-um-nommu
```
This is running the image at `tests/docker/linux-um-nommu/Dockerfile` which is using `alpine-test.ext3` which is built using the `alpine-test.ext3` target
in `tests/Makefile` (which is based on the image at `tests/docker/alpine/Dockerfile`).

# Demo:

[![asciicast](https://asciinema.org/a/343173.svg)](https://asciinema.org/a/343173)

This shows a run with the host syscalls on the left.
