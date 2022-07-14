"""
This script will:
 > Open GUI which will be able to open, change and save a flight plan
 > Open file by clicking "Open a file" and navigate to the flight plan
 > Change can be done by filling in the coordinates manually by filling in longitude, 
    latitude and an optional "heading" (if the mission needs to be rotated counter-clcokwise)
 > You can also paste them from clipboard (copy coordinates via google maps and click "Paste")
 > A google maps link can also be used to change the flight plan (copy the link and select "Import from URL")
 > When the parameters have been filled in, press "Mission change" to change the flight plan
 > Saving can be done by pressing "Save plan". In there, you can either create a new plan or overwrite the old plan
"""

import json
import tkinter as tk
from tkinter import ttk
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo
from jsontest2 import get_latlon, mv_waypoints, rot_waypoints
import clipboard

# Create window
root = tk.Tk()
root.title('QGC mission changer')
root.resizable(False,False)
root.geometry('700x250')


def select_file():

    global full_plan
    global file_path

   # Which files can be seen in explorer
    filetypes = (
        ('plan files','*.plan'),
        ('All files','.*')
    )
    
    # Open file explorer
    file_path = fd.askopenfilename(
        title='Open a file',
        initialdir='/home/dennism/mavlink',
        filetypes=filetypes
    )

    # Show which file is selected
    showinfo(
        title='Selected file',
        message=file_path
    )
    
    # Open file
    with open(file_path, 'r') as file_object:
        data = json.load(file_object)
    
    # Retrieve the data
    full_plan = data
    LLocation.configure(text="File path: "+file_path)
    LSave.configure(text="")
    text_button.configure(text="Mission change")

def mission_changer():

    global full_plan
    try:
        # Get heading if filled in
        heading = int(THead.get("1.0","end-1c"))
    except:
        print("no heading")
    try:
        # Get latitude
        lat = float(TLat.get("1.0","end-1c"))
    except:
        print("no lat")
    try:
        # Get longitude
        lon = float(TLon.get("1.0","end-1c"))
    except:
        print("no lon")
    try:
        if heading > 0:
            # Only rotate if heading has been filled in
            full_plan = rot_waypoints(file_path, heading)
        # Move waypoints
        full_plan = mv_waypoints(file_path, lat, lon)
    except:
        print("something went wrong!")
    LSave.configure(text="")
    # Successfully changed
    text_button.configure(text="Mission changed!")

def paste():
    # Empty latitude and longitude text box
    TLat.config(state=tk.NORMAL)
    TLon.config(state=tk.NORMAL)
    TLat.delete('1.0', tk.END)
    TLon.delete('1.0', tk.END)
    
    # Get clipboard data
    pasted = clipboard.paste()
    
    # Split latitude and longitude and fill them in
    floats = pasted.split(", ")
    TLat.insert(tk.END, floats[0])
    TLon.insert(tk.END, floats[1])

def from_url():
    try:
        # Get google link
        s = clipboard.paste()
        found = s.find("www.google.com/maps")
        if found != -1:
            # Go to the location of the coordinates in link
            print("found! : " + s)
            found = s.find('@')
            s = s[found+1:]
            
            # Get latitude value
            lat = s[:s.find(',')]
            s = s[s.find(',')+1:]
            
            # Get longitude value
            lon = s[:s.find(',')]
            
            # Empty the latitude and longitude text box
            TLat.config(state=tk.NORMAL)
            TLon.config(state=tk.NORMAL)
            TLat.delete('1.0', tk.END)
            TLon.delete('1.0', tk.END)
            
            # Fill in latitude and longitude
            TLat.insert(tk.END, lat)
            TLon.insert(tk.END, lon)
        else:
            print("no correct google url found!")
    except:
        print("something went wrong")

def file_save():
    # Open explorer to save file
    save_name=fd.asksaveasfile(mode='w',defaultextension=".plan")
    if save_name is None:
        return
     
    # Save file
    file2save = json.dumps(full_plan)
    save_name.write(file2save)
    save_name.close
    LSave.configure(text="Successfully saved!")
    text_button.configure(text="Mission change")

# Open file button
open_button = ttk.Button(
    root,
    text="Open a file",
    command=select_file
)

open_button.grid(column=0,row=1,sticky='w',padx=10,pady=10)

# Mission change button
text_button = ttk.Button(
    root,
    text="Mission change",
    command=mission_changer
)

text_button.grid(column=0,row=2,sticky='w',padx=10,pady=10)

# Paste button
paste_button = ttk.Button(
    root,
    text="Paste",
    command=paste
)

paste_button.grid(column=7,row=2,sticky='w',padx=10,pady=10)

# Import from URL button
from_url_button = ttk.Button(
    root,
    text="Import from URL",
    command=from_url
)

from_url_button.grid(column=0,row=3,sticky='w',padx=10,pady=10)

# Save plan button
save_button = ttk.Button(
    root,
    text="Save plan",
    command=file_save
)

save_button.grid(column=0,row=4,sticky='w',padx=10,pady=10)

# All the labels and text
LLocation = tk.Label(root, text='File path: ')
LLocation.grid(column=1,row=1,sticky='w',padx=10,pady=1,columnspan=6)

LHead = tk.Label(root, text='heading: ')
LHead.grid(column=1,row=2,sticky='w',padx=10,pady=10,columnspan=1)
THead = tk.Text(root, width=5, height=1)
THead.grid(column=2,row=2,sticky='e',padx=10,pady=10,columnspan=1)

THead.bind('<Control-v>', lambda _:'break')

LLat = tk.Label(root, text='lat: ')
LLat.grid(column=3,row=2,sticky='w',padx=10,pady=10,columnspan=1)
TLat = tk.Text(root, width=10, height=1)
TLat.grid(column=4,row=2,sticky='e',padx=10,pady=10,columnspan=1)

LLon = tk.Label(root, text='lon: ')
LLon.grid(column=5,row=2,sticky='w',padx=10,pady=10,columnspan=1)
TLon = tk.Text(root, width=10, height=1)
TLon.grid(column=6,row=2,sticky='e',padx=10,pady=10,columnspan=1)

LSave = tk.Label(root, text='')
LSave.grid(column=1,row=3,sticky='w',padx=10,pady=10,columnspan=6)

root.mainloop()
