# vl53l0 csv

import csv 

def main():
    x = 0
    # CHANGE THIS TO THE ACTUAL PATH IF YOU WANT TO RUN THIS FILE
    path = "/home/wizard/ENR-259-Engineering-Design-Lab/SensorTestCode/vl53l0-test/output.txt"
    with open(path, 'r') as raw_data:
        stripped = (line.strip() for line in raw_data)
        lines = (line.split(",") for line in stripped if line)
        with open('output.csv', 'w') as output_file:
            writer = csv.writer(output_file)
            writer.writerow(['distance'])
            writer.writerows((lines))

if __name__ == "__main__":
    main()
