sudo docker build . -t optimizer-server:latest
sudo docker run -dp 5000:50051 optimizer-server:latest
sudo docker logs -f $(sudo docker ps -q | head -n 1)

