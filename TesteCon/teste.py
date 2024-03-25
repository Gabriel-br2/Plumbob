import requests

ip_esp32 = "192.168.2.111"

url = f"http://{ip_esp32}/mood_data"

data = {"red": str(255),"green": str(255), "blue": str(0)}
response = requests.post(url, data=data)

print(response.text)
