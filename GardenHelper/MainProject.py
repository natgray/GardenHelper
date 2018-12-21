#Main GUI for garden Helper PC interface
#Author: Monica Barrios Hernandez
from tkinter import *
from tkinter import ttk

#clickMe
def clickMe1():
    LowT = LowTempBox.get()
    t1 = open("t1.txt", "a")
    t1.write(LowT)
    t1.write(" ")
    t1.close()

def clickMe2():
    HighT = HighTempBox.get()
    t1 = open("t1.txt", "a")
    t1.write(HighT)
    t1.write(" ")
    t1.close()

def clickMe3():
    LowM = LowMoistBox.get()
    text_file = open("t1.txt", "a")
    text_file.write(LowM)
    text_file.write(" ")
    text_file.close()

def clickMe4():
    HighM = HighTempBox.get()
    text_file = open("t1.txt", "w")
    text_file.write("hello!")
    text_file.write(" ")
    text_file.close()

#Main:
window = Tk()
window.title("Garden Helper")
window.configure(background="olive")
window.geometry('500x500')
window.resizable(0,0)

#LOGO image
logo = PhotoImage(file= 'Garden Helper\myNewLogo.png')
labelLogo = Label(window, image = logo)
labelLogo.grid(row = 1, column = 3 )

rows = 0
while rows < 50:
    window.rowconfigure(rows, weight =1)
    window.columnconfigure(rows, weight = 1)
    rows+=1


nb = ttk.Notebook(window)
nb.grid(row = 10, column = 0, columnspan = 50, rowspan = 49, sticky = 'NESW')

page1 = ttk.Frame(nb)
nb.add(page1, text='Setup')

page2 = ttk.Frame(nb)
nb.add(page2, text = 'Graphs')

#Labels
TempL = Label(page1, text = "Enter minimum temperature: ", font = "verdana 12 bold")
TempL.grid(row=1,column=0, sticky=W)
TempH = Label(page1, text = "Enter maximum temperature: ", font = "verdana 12 bold")
TempH.grid(row=4,column=0, sticky=W)
MoistL= Label(page1, text = "Enter the minimum moisture percentage: ", font = "verdana 12 bold")
MoistL.grid(row =7, column = 0 , sticky =W)
MoistH= Label(page1, text = "Enter the maximum moisture percentage: ",font = "verdana 12 bold")
MoistH.grid(row =9, column = 0 , sticky =W)

#Text Entry
LowTempBox= Entry(page1, bg="white")
LowTempBox.grid(row= 2, column= 0, sticky =W)
HighTempBox= Entry(page1, bg = "white")
HighTempBox.grid(row = 5, column = 0, sticky=W)
LowMoistBox= Entry(page1, bg = "white")
LowMoistBox.grid(row = 8, column=0, sticky=W)
HighMoistBox = Entry(page1,bg="white")
HighMoistBox.grid(row=10, column=0, sticky=W)

# Save Buttons
SaveLowTemp = Button(page1, text="Save", command=clickMe1)
SaveLowTemp.grid(column=0, row=2)
SaveHighTemp = Button(page1, text="Save", command=clickMe2)
SaveHighTemp.grid(column=0, row=5)
SaveLowMoist = Button(page1, text="Save", command=clickMe3)
SaveLowMoist.grid(column=0, row=8)
SaveHighMoist = Button(page1, text="Save", command=clickMe4)
SaveHighMoist.grid(column=0, row=10)

window.mainloop()
