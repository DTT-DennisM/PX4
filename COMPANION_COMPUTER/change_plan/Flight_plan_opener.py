import json
import tkinter as tk
from tkinter import ttk
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo
from jsontest2 import get_latlon, mv_waypoints, rot_waypoints
import clipboard

root = tk.Tk()
root.title('QGC mission changer')
root.resizable(False,False)
root.geometry('700x250')



def select_file():

    global full_plan
    global file_path

    filetypes = (
        ('plan files','*.plan'),
        ('All files','.*')
    )

    file_path = fd.askopenfilename(
        title='Open a file',
        initialdir='/home/dennism/mavlink',
        filetypes=filetypes
    )

    showinfo(
        title='Selected file',
        message=file_path
    )

    with open(file_path, 'r') as file_object:
        data = json.load(file_object)

    full_plan = data
    LLocation.configure(text="File path: "+file_path)
    LSave.configure(text="")
    text_button.configure(text="Mission change")

def mission_changer():

    global full_plan
    try:
        heading = int(THead.get("1.0","end-1c"))
    except:
        print("no heading")
    try:
        lat = float(TLat.get("1.0","end-1c"))
    except:
        print("no lat")
    try:
        lon = float(TLon.get("1.0","end-1c"))
    except:
        print("no lon")
    try:
        if heading > 0:
            full_plan = rot_waypoints(file_path, heading)
        full_plan = mv_waypoints(file_path, lat, lon)
    except:
        print("smth went wrong!")
    LSave.configure(text="")
    text_button.configure(text="Mission changed!")
    #except:
        #print("at least 1 field left blank")
        #pass
    #full_plan["mission"]["items"][1]["doJumpId"] = 10

def paste():
    #try:
        TLat.config(state=tk.NORMAL)
        TLon.config(state=tk.NORMAL)
        TLat.delete('1.0', tk.END)
        TLon.delete('1.0', tk.END)
        pasted = clipboard.paste()
        #print(pasted)
        floats = pasted.split(", ")
        TLat.insert(tk.END, floats[0])
        TLon.insert(tk.END, floats[1])
        print(floats[0])
        print(floats[1])
    #except:
    #    print("nono")
    #    pass

def from_url():
    try:
        s = clipboard.paste()
        print(s)
        found = s.find("www.google.com/maps")
        if found != -1:
            print("found! : " + s)
            found = s.find('@')
            s = s[found+1:]
            print(s)
            lat = s[:s.find(',')]
            s = s[s.find(',')+1:]
            print(s)
            lon = s[:s.find(',')]
            TLat.config(state=tk.NORMAL)
            TLon.config(state=tk.NORMAL)
            TLat.delete('1.0', tk.END)
            TLon.delete('1.0', tk.END)
            TLat.insert(tk.END, lat)
            TLon.insert(tk.END, lon)
            print(lat)
            print(lon)
        else:
            print("no correct google url found!")
    except:
        print("smth went wrong hiero")
    print("from url!")

def file_save():
    save_name=fd.asksaveasfile(mode='w',defaultextension=".plan")
    if save_name is None:
        return
    file2save = json.dumps(full_plan)
    save_name.write(file2save)
    save_name.close
    LSave.configure(text="Successfully saved!")
    text_button.configure(text="Mission change")

# Button
open_button = ttk.Button(
    root,
    text="Open a file",
    command=select_file
)

open_button.grid(column=0,row=1,sticky='w',padx=10,pady=10)

# Button 2
text_button = ttk.Button(
    root,
    text="Mission change",
    command=mission_changer
)

text_button.grid(column=0,row=2,sticky='w',padx=10,pady=10)

# Button 3
paste_button = ttk.Button(
    root,
    text="Paste",
    command=paste
)

paste_button.grid(column=7,row=2,sticky='w',padx=10,pady=10)

# Button 5
from_url_button = ttk.Button(
    root,
    text="Import from URL",
    command=from_url
)

from_url_button.grid(column=0,row=3,sticky='w',padx=10,pady=10)

# Button 4
save_button = ttk.Button(
    root,
    text="Save plan",
    command=file_save
)

save_button.grid(column=0,row=4,sticky='w',padx=10,pady=10)


LLocation = tk.Label(root, text='File path: ')
LLocation.grid(column=1,row=1,sticky='w',padx=10,pady=1,columnspan=6)

LHead = tk.Label(root, text='heading: ')
LHead.grid(column=1,row=2,sticky='w',padx=10,pady=10,columnspan=1)
THead = tk.Text(root, width=5, height=1)
THead.grid(column=2,row=2,sticky='e',padx=10,pady=10,columnspan=1)

#floats = list(map(float, clipboard.paste().split()))
#print(floats)
THead.bind('<Control-v>', lambda _:'break')
"""
EHead = tk.Entry(root, exportselection=0)
EHead.insert(0, "Type words here!")
EHead.bind('<Control-v>', lambda _:'break')
EHead.grid(column=2,row=2,sticky='e',padx=10,pady=10,columnspan=1)
"""

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
#file_path = fd.askopenfilename()

'''
print(file_path)


#files = 'Transition_test.plan'

with open(file_path, 'r') as file_object:
    data = json.load(file_object)
    print(data["mission"]["items"][0]["params"][4:6])


# replace a value
data["mission"]["items"][0]["params"][4:6] = [52, 6]
print(data["mission"]["items"][0]["params"][4:6])
'''

root.mainloop()
