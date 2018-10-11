### Make the program
make

### Build the Docker Image
Build the docker image with the following command in the source code directory
```
docker build -t merge-json:latest .
```
Notice the "." at the end of the command above

### Run the Docker Image
docker run --rm -v $(pwd):/data \
-e GUEST_INPUT_FILE=input.json \
-e GUEST_OUTPUT_FILE=/data/output.json \
merge-json:latest

# docker-json-merge
Merge contents of input json file into output json file