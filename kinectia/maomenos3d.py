import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import pandas as pd

# Your list of XYZ points
points = [
    428, 232, 0.0, 428, 262, 0.0, 403, 268, 1645.387329, 427, 284, 0.0,
    409, 303, 0.0, 410, 350, 0.0, 415, 358, 0.0, 397, 359, 0.0, 397, 237, 0.0,
    377, 255, 0.0, 379, 227, 3572.471924, 407, 284, 0.0, 378, 297, 0.0, 386, 340, 0.0,
    392, 347, 0.0
]

# Reshape the list into a NumPy array with three columns
points_array = np.array(points).reshape(-1, 3)

print(points_array)

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Scatter plot the points
ax.scatter(points_array[:, 0], points_array[:, 1], points_array[:, 2], c='r', marker='o')

# Set axis labels
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

# Show the plot
plt.show()
