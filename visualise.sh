sudo docker cp $(sudo docker ps -q | head -n 1):/Optimizer/cuboids_to_render.xml ./visual/app/cuboids_to_render.xml
cd ./visual/app/ && python3 -m http.server 8005 
