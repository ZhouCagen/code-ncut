import  matplotlib.pyplot as plt
monthly_temperatures = [5, 7, 9, 12, 15, 17, 16, 14, 10, 8, 6, 5, 4, 6, 8, 11, 13, 15, 18, 20, 22, 23, 21, 19, 17, 15, 12, 10, 8]
days_of_month = list(range(1, 31))
plt.title("Monthly Average Temperatures")
plt.xlabel("Day of Month")
plt.ylabel("Average Temperature (°C)")
plt.plot(monthly_temperatures,marker='o')
plt.show()