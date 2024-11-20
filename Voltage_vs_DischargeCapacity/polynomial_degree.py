import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error

# Example data
x = np.array([3.0, 3.2, 3.4, 3.6, 3.8, 4.0])
y = np.array([0, 20, 40, 60, 80, 100])

# Test polynomial degrees
max_degree = 10
errors = []

for degree in range(1, max_degree + 1):
    coefficients = np.polyfit(x, y, degree)
    polynomial = np.poly1d(coefficients)
    y_pred = polynomial(x)
    error = mean_squared_error(y, y_pred, squared=False)  # RMSE
    errors.append(error)

# Plot RMSE vs. Degree
plt.plot(range(1, max_degree + 1), errors, marker='o')
plt.xlabel('Polynomial Degree')
plt.ylabel('RMSE')
plt.title('Selecting the Best Polynomial Degree')
plt.show()

# Best degree (minimum error)
best_degree = np.argmin(errors) + 1
print("Best Polynomial Degree:", best_degree)
