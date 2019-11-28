 This is just docker mini how-to, just skip this if you are not planning to test HDC inside docker.

# Build image
docker build -t pokus-xenial .

# "Run" image instance
docker run --name pokus_instance -i -t pokus

# Delete image instance
docker container rm pokus_instance

# Push image to IPP dockerhub
docker tag pokus dockerhub.tok.ipp.cas.cz:5000/pokus
docker push dockerhub.tok.ipp.cas.cz:5000/pokus
