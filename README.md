🛒 Smart Trolley Using RFID
A smart shopping trolley system that uses RFID (Radio Frequency Identification) technology to automate billing and reduce checkout time in retail stores. This project aims to enhance the shopping experience by allowing customers to scan items as they shop, view the bill in real-time, and complete payment without standing in queues.

🚀 Features
📦 RFID-Based Scanning – Items are scanned using RFID tags and automatically added to the cart.

📱 Real-Time Billing – Total cost updates live as new items are added or removed.

🧾 LCD Display / Mobile App – Shows item details and total cost instantly.

⛔ Anti-Theft Detection – Prevents unbilled item removal through RFID gate alert system.

🔋 Battery Powered & Portable – Compact system suitable for real-time shopping.

🧑‍💻 Technologies Used
Component	Technology/Hardware
Microcontroller	Arduino Uno / NodeMCU
Scanning	RFID Reader (RC522)
Tagging	RFID Tags
Display	16x2 LCD / OLED / App UI
Communication	Bluetooth / Wi-Fi Module
Power Supply	Battery or 9V Adapter
Optional	Buzzer, IR Sensor (Anti-Theft)

🧠 Working Principle
Each product is tagged with an RFID tag.

When a user places a product in the trolley, the RFID reader detects the tag and sends the data to the microcontroller.

The microcontroller fetches product details from a database (or pre-coded array) and updates the total amount on the display.

At the billing counter, the data can be transmitted wirelessly to complete payment or clear the trolley.

📂 Project Structure
Copy
Edit
smart-trolley-rfid/
│
├── ArduinoCode/
│   ├── smart_trolley.ino
│   └── product_data.h
│
├── circuit_diagram/
│   └── schematic.png
│
├── docs/
│   └── presentation.pdf
│
├── README.md
└── images/
    ├── prototype.jpg
    └── lcd_demo.png# Smart-Trolley-using-RFID
A Smart Trolley using RFID is an automated shopping cart that uses Radio-Frequency Identification (RFID) to identify and track products placed inside it. This system helps streamline the shopping experience by automatically calculating the total bill, reducing checkout time, and preventing theft.
