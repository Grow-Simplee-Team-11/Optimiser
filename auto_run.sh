sudo docker build . -t optimizer-server:test
sudo docker run -dp 5002:50051 optimizer-server:test
sudo docker logs -f $(sudo docker ps -q | head -n 1)

