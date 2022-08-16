# Code to read the data from Arduino Serial Monitor

# Importing required libraries
import serial
import time
import schedule
import pandas as pd


# Function to read the data
def main_func():

    # Empty lists to store the readings of each house
    house1 = []
    house2 = []
    house3 = []
    # Declaring Dataframe with the following columns
    my_df = pd.DataFrame(columns=['Time','House1', 'House2', 'House3', 'Action'])

    for i in range(50):
        # Declaring object of class Serial
        arduino = serial.Serial('com5', 115200)
        print('Established serial connection to Arduino')
        # Storing the data in variable
        arduino_data = arduino.readline()
        print(arduino_data)
        # Decoding the data so that the data can be processed in Python language
        decoded_values = str(arduino_data[0:len(arduino_data)].decode("utf-8"))

        list_values = decoded_values.split(' ')

        # Logic to determine whether the bin is empty, half or full
        if int(list_values[1]) <= 30:
            house1.append("High")
            print("H")
        if int(list_values[1]) > 30 and int(list_values[1]) < 70:
            house1.append("Medium")
            print("m")
        if int(list_values[1]) > 70:
            house1.append("Low")
            print("l")

        if int(list_values[3]) <= 30:
            house2.append("High")
            print("H")
        if int(list_values[3]) > 30 and int(list_values[3]) < 70:
            house2.append("Medium")
            print("m")
        if int(list_values[3]) > 70:
            house2.append("Low")
            print("l")

        if int(list_values[5]) <= 30:
            house3.append("High")
            print("H")
        if int(list_values[5]) > 30 and int(list_values[5]) < 70:
            house3.append("Medium")
            print("m")
        if int(list_values[5]) > 70:
            house3.append("Low")
            print("l")

        time.sleep(5)

        arduino_data = 0
        list_in_floats.clear()
        list_values.clear()
        arduino.close()
        print('Connection closed')
        print('<----------------------------->')
    my_df['House1'] = house1
    my_df['House2'] = house2
    my_df['House3'] = house3

    for i in range(len(my_df)):
        # Logic to check whether trash pickup should be done from the respective locality
        if (my_df['House1'][i] == 'High' or my_df['House2'][i] == 'High' or my_df['House3'][i] == 'High'):
            my_df['Action'][i] = "PICKUP"
            if i == len(my_df)-1:
                break
            else:
                my_df['House1'][i+1], my_df['House2'][i+1], my_df['House3'][i+1] = 'Low', 'Low', 'Low'

        if ((my_df['House1'][i] == 'Medium' and my_df['House2'][i] == 'Medium')
            or (my_df['House2'][i] == 'Medium' and my_df['House3'][i] == 'Medium')
            or (my_df['House1'][i] == 'Medium' and my_df['House3'][i] == 'Medium')):

            my_df['Action'][i] = "PICKUP"

            if i == len(my_df)-1:
                break
            else:
                my_df['House1'][i+1], my_df['House2'][i+1], my_df['House3'][i+1] = 'Low', 'Low', 'Low'

    # Creating CSV file of our data
    my_df.to_csv("./our_data.csv", index=False)


# ----------------------------------------Main Code------------------------------------

if __name__ == '__main__':
    list_values = []
    list_in_floats = []

    print('Program started')

    main_func()




