import requests
import time
import json

# modify_plumboobIP 000.000.0.000
# plumboob_status
# plumboob_connection
# plumboob_batery_status 

dic = {
    "Angry":         "C3192B",
    "Uncomfortable": "E26246",
    "Tense":         "DF841C",
    "Embarassed":    "E1C043",
    "Energized":     "9DC948",
    "Happy":         "28B552",
    "Inspired":      "33BCC1",
    "Confident":     "448CC8",
    "Sad":           "2C44AA",
    "Focused":       "7038EC",
    "Dazed":         "816DCC",
    "Playful":       "B646AD",
    "Flirty":        "EE5DA5",
    "Scared":        "7E1260",
    "Bored":         "818785",
    "Fine":          "E9E9E9",
    "Asleep":        "4D4D70",
    "Possessed":     "4D4D70"
    }

CONFIG_FILE = "config.json"
current_color = None
running = True

def load_config():
    try:
        with open(CONFIG_FILE, "r") as file:
            config = json.load(file)
            return config.get("esp32_ip")
    except (FileNotFoundError, json.JSONDecodeError):
        return None

def save_config(esp32_ip):
    config = {"esp32_ip": esp32_ip}
    with open(CONFIG_FILE, "w") as file:
        json.dump(config, file)

def set_esp32_ip():
    esp32_ip = input("Digite o novo endereço IP do ESP32 (xxx.xxx.x.xxx): ")
    save_config(esp32_ip)
    return esp32_ip

def get_bateryStat(esp32_ip):
    url = f"http://{esp32_ip}/getBatery"
    
    response = requests.post(url)
    print("batery: ", response.text)

def send_color(color, esp32_ip, current_color):
    if color == current_color: return

    url = f"http://{esp32_ip}/setcolor"
    payload = {"hex": color}

    response = requests.post(url, data=payload)
    print(response.text)

    c = 0
    while (response.text != 'OK'):
        c += 1
        url = f"http://{esp32_ip}/setcolor"
        payload = {"hex": color}

        print(url)
        print(payload)
        response = requests.post(url, data=payload)
    
        if c >= 100:
            print("giving up, check connection")
            break

    if response.text != 'OK':
        current_color = color    

esp32_ip = load_config()
if esp32_ip is None:
    esp32_ip = '000.000.0.000'
    save_config(esp32_ip)

while True:
    try: 
        mood = input("Digite a emoção: ")

        if mood.lower() == 'modify_plumboobip': 
            esp32_ip = set_esp32_ip()

        elif mood.lower() == 'plumboob_connection false':
            running = False

        elif mood.lower() == 'plumboob_connection true':
            running = True

        elif mood.lower() == 'plumboob_status': 
            print(running, current_color, esp32_ip)

        elif mood.lower() == 'plumboob_batery_status':
            get_bateryStat(esp32_ip)

        elif running:
            color = dic[mood]
            send_color(color, esp32_ip, current_color)
            time.sleep(1)
            
    except Exception as e:
        print(e)