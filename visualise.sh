sudo docker cp $(sudo docker ps -q | head -n 1):/Optimizer/tests/cluster.txt ./cluster.txt
