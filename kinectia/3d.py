import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

# Read the CSV file into a Pandas DataFrame
df = pd.read_csv('movimiento.csv', header=None)

# Initialize frame index
frame_index = 0

def plot_frame(frame_index, ax):
    ax.clear()

    # Extract the points for the current frame
    start_idx = frame_index
    end_idx = (frame_index + 1)
    frame_points = df.iloc[start_idx:end_idx].values.reshape(-1, 3)

    # Scatter plot the points for the current frame
    ax.scatter(frame_points[:, 0], frame_points[:, 1], frame_points[:, 2], c='r', marker='o')

    # Connect points with lines
    ax.plot(frame_points[:, 0], frame_points[:, 1], frame_points[:, 2], c='b', linestyle='-', linewidth=2)

    # Set axis labels
    ax.set_xlabel('X Label')
    ax.set_ylabel('Y Label')
    ax.set_zlabel('Z Label')

def update(frame_index):
    # Plot the current frame
    plot_frame(frame_index, ax)

# Set up the initial plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Create the animation
animation = FuncAnimation(fig, update, frames=len(df), interval=1000, repeat=True)

# Show the plot
plt.show()