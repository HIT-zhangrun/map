import folium
import json

with open("special_height_zones.json", "r") as zones:
    lat_lng = json.load(zones)

map = folium.Map(location = [34.12052, 107.256486],
        zomm_start = 3,
        control_scale = True,
        titles = 'Stamen Terrain'
        )
choose_color = ['red', 'blue', 'green']

num = -1
lat = [0, 0]
dis = [0]

for zone in lat_lng:
    if (num % 2 != 0):
        lat = zone
        lat = lat[::-1]
    else:
        dis = zone
        print(lat, dis)
    num = num + 1
    folium.Circle(location = lat,
            radius = dis,
            color = 'blue',
            fill = True,).add_to(map)

map.add_child(folium.LatLngPopup())
map.save("special_height_zones.html")



