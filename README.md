# docker-json-merge
Merge contents of input json file into output json file.

### Make the program
Make the program before building docker image.
```
make
```

### Build the Docker Image
Build the docker image with the following command in the source code directory.
```
docker build -t merge-json:latest .
```
Notice the "." at the end of the command above.

### Run the Docker Image
This program is fully dockerized and intended to run only using docker
```
docker run --rm -v $(pwd):/data \
-e GUEST_INPUT_FILE=input.json \
-e GUEST_OUTPUT_FILE=/data/output.json \
merge-json:latest
```