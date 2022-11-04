import folium
import json

with open("special_height_zones.json", "r", encoding='utf-8') as zones:
    lat_lng = json.load(zones)

map = folium.Map(location = [34.12052, 107.256486],
        zomm_start = 3,
        control_scale = True,
        titles = 'Stamen Terrain'
        )
choose_color = ['red', 'blue', 'green']

num = 0
lat = [0, 0]
dis = [0]
name = ["0"]

for zone in lat_lng:
    if (num % 3 == 0):
        lat = zone
        lat = lat[::-1]
    elif (num % 3 == 1):
        dis = zone
    elif (num % 3 == 2):
        name = zone
        print(lat, dis, name)
        folium.Circle(location = lat,
            radius = dis,
            color = 'blue',
            fill = True,).add_to(map)
        folium.Marker(location = lat,
            popup = name,
            icon = folium.Icon(icon = 'cloud')).add_to(map)
    num = num + 1
map.add_child(folium.LatLngPopup())
map.save("special_height_zones.html")



