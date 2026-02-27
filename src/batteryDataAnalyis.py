"""
@file batteryDataAnalyis.py

@brief A quick script that takes in a battery data script and performs calculations upon it.
Will calculate the power used at any given moment and will then find the average capacity of that battery in 
watt hours.

@author Wesley Campbell
@date 2026-02-26 
@version 1.0.0
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

data_dir = "~/Code/byu/lightTrackingRobot/data"
data_file= data_dir + "/9V_battery_sheet.csv"

data_file_columns = ["Time (hours)", "Voltage"]

RESISTANCES = [200] 

battery_capacities = {}

def import_data(file_path, columns):
    df = pd.read_csv(file_path, usecols=columns)
    return df

def save_data(df, filename):
    df.to_csv(data_dir + "/" + filename + '.csv')

def power(voltage, resistance):
    return voltage * voltage / resistance;

def midpoint_time_values(time_column):
    delta_times1 = []
    for i, time in enumerate(time_column):
        delta_times1.append(time - time_column[i - 1] if i > 0 else 0)
    delta_times2 = []
    for i, time in enumerate(time_column):
        delta_times2.append(time_column[i + 1] - time if i < len(time_column) - 1 else 0)

    delta_times = [(x + y) / 2 for x, y in zip(delta_times1, delta_times2)]

    return delta_times

def batteryCapacity(df_time_column, df_power_column):
    delta_times = midpoint_time_values(df_time_column)

    capacity = 0
    for d_time, power in zip(delta_times, df_power_column):
        capacity += power * d_time; 

    return float(capacity)

def graph_data_frame(df):
    x = data_file_columns[0]


    fig, ax = plt.subplots()

    for col in df.columns:
        if col != x:
            ax.plot(df[x], df[col], label=col, marker='.')

    plt.xlabel(x)
    ax.yaxis.set_major_locator(ticker.MultipleLocator(0.5))

    ax.grid(True)

    plt.legend()
    plt.show()


if __name__ == "__main__":
    df = import_data(data_file, data_file_columns)

    for resistance in RESISTANCES:
        power_df = df['Voltage'].apply(lambda x: power(x, resistance))

        df[f"Power {resistance}\u2126 (Wh)"] = power_df
        battery_capacities[resistance] = batteryCapacity(df[data_file_columns[0]], power_df)

    print("Data Set:   ")
    print(df)
    
    save_file = ''
    while save_file not in ['y', 'n']:
        save_file = input("Save data? [y,n] ")

    if save_file == 'y':
        filename = input("    input filename: ")
        save_data(df, filename)

    print("Graphing data...")
    graph_data_frame(df)
