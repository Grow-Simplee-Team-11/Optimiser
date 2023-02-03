sudo docker cp $(sudo docker ps -q):/Optimizer/cuboids_to_render.xml ./visual/app/cuboids_to_render.xml
cd ./visual/app/ && python3 -m http.server 8005 
