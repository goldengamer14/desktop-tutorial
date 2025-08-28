from json import loads

with open("Maaz-Python\\Openpyxl_Training\\Exercises.json", "r") as file:
    data = loads(file.read())
    print(data, type(data))