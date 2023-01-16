import folium
import random
import distinctipy


used_colors = set([])
def randomColor():
    global used_colors
    i = 0
    color = "#"+''.join([random.choice('0123456789ABCDEF') for j in range(6)])
    while (color in used_colors) and (i<10):
        i = i+1
        color = "#"+''.join([random.choice('0123456789ABCDEF') for j in range(6)])

    if(i>=10):
        raise "Ran out of tries for colors"
    
    used_colors.add(color)
    return color
    
if __name__=="__main__":
    random.seed(10)

    pts = open("input.txt", "r")
    # depot location
    loc = pts.readline()
    depot_x_loc = float(loc.split(" ")[0])
    depot_y_loc = float(loc.split(" ")[1])

    clusters = [int(v) for v in open('output.txt').read().split()]
    print(clusters)
    

    pts.readline() # the dimensions of the bin
    num_ct = int(pts.readline()) # the number of packages
    colors = distinctipy.get_colors(len(set(clusters)))
    print(colors)
    temp = []
    for color in colors:
        temp.append(tuple(int(255 * elem) for elem in color))

    print(temp)

    results = []
    map = folium.Map(location=[42.150, -88.034])
    folium.Marker(location=[depot_x_loc, depot_y_loc], popup =  'Depot').add_to(map)


    for i in range(num_ct):
        line = pts.readline()
        pt_x_loc = float(line.split(" ")[0])
        pt_y_loc = float(line.split(" ")[1])
        
        # results.append([pt_x_loc, pt_y_loc, colors[clusters[i]]])
        folium.Circle(
            radius = 50, 
            location=[pt_x_loc, pt_y_loc],
            fill = False,
            popup = f"lat={pt_x_loc}, lng={pt_y_loc}, index={i}",
            color = str('#{:X}{:X}{:X}').format(temp[clusters[i]][0], temp[clusters[i]][1], temp[clusters[i]][2])
        ).add_to(map)
        print(('#{:X}{:X}{:X}').format(temp[clusters[i]][0], temp[clusters[i]][1], temp[clusters[i]][2]))

    map.save('clusters.html')
