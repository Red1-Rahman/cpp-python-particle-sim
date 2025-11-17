import matplotlib.pyplot as plt
import matplotlib.animation as animation
import json
import os

emitter_x, emitter_y = 0.0, 0.0

def load_particles(filename="particles.json"):
    if not os.path.exists(filename):
        return []
    with open(filename, "r") as f:
        try:
            return json.load(f)
        except:
            return []

fig, ax = plt.subplots()
ax.set_xlim(-10, 10)
ax.set_ylim(0, 10)
scat = ax.scatter([], [], s=50)

# semi-transparent trails
fig.patch.set_alpha(0.9)
ax.patch.set_alpha(0.05)

def update(frame):
    particles = load_particles()
    x = [p["x"] for p in particles]
    y = [p["y"] for p in particles]
    colors = [(p["r"], p["g"], p["b"], p["a"]) for p in particles]
    scat.set_offsets(list(zip(x, y)))
    scat.set_color(colors)
    return scat,

ani = animation.FuncAnimation(fig, update, interval=50)

# user-controlled emitter
def on_key(event):
    global emitter_x, emitter_y
    if event.key == 'left':
        emitter_x -= 1
    elif event.key == 'right':
        emitter_x += 1
    elif event.key == 'up':
        emitter_y += 1
    elif event.key == 'down':
        emitter_y -= 1

fig.canvas.mpl_connect('key_press_event', on_key)
plt.show()
