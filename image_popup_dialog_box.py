#opens dialog box, has image, for main page, 



from Tkinter import *
import Tkinter
import Tkinter as tk
from PIL import Image, ImageTk
import os

import ttk

#master = Tk()


#def create_window():

	#global frame 
	
	
	#photo = ImageTk.PhotoImage(Image.open("C:\Users\g00238234\Desktop\Ryu.png"));
	#frame.create_image(10, 10, image = photo, anchor = SW)
	#image = Image.open("C:\Users\g00238234\Desktop\Ryu.png")
	#photo = ImageTk.PhotoImage(image)
	#label = Label(master,image=photo,bg="yellow")
	#label.image = photo # keep a reference!
	#label.pack()
	

	
	#window = tk.Toplevel(root)
	
def create_dialog():
		window = tk.Toplevel(root)

	
def choose_combo():

		global canvas 
		global item
		
		start_button.destroy()
		
		FILENAME = 'U:\year5\Project\choose_combo.gif'
		
		tk_img2 = ImageTk.PhotoImage(file = FILENAME)
		canvas.image = tk_img2
		item = canvas.create_image(200, 50, image=tk_img2, anchor = 'n')
		
		igm2 = PhotoImage(file="U:\year5\Project\Menu.gif")
		Menu_btn = tk.Button(root, text ="Menu", command = create_dialog, anchor = 'w',
                    width = 10, activebackground = "#33B5E5")
		menu_button_window = canvas.create_window(10, 10, anchor='nw', window=Menu_btn) 

		igm3 = PhotoImage(file="U:\year5\Project\combo1.gif")
		root.image = igm3
		combo_button = tk.Button(root, image=igm3, command = choose_combo, anchor = 'nw',
                    width = 700, activebackground = "#33B5E5")
		combo_button_window = canvas.create_window(80, 120, anchor='w', window=combo_button)  
		
		
			
		
		
		
		
		
		
		
		
		
#w = ttk.Progressbar(master,length=800,orient=HORIZONTAL)



#frame = Canvas(root, width=500, height=50)
#photo = 

#frame.create_image(10, 10, image = photo, anchor = NW)

#image = Image.open("C:\Users\g00238234\Desktop\abel-street-fighter-streetfighter-arcade-games-1920x1080-wallpaper28967.jpg")
#backgroundImage=ImageTk.PhotoImage(image)

#label = Label(frame,bg="yellow")
#btn = Button(root, text = "Start", command=choose_combo)
#btn.grid(row=3, column=0)
#btn.pack()



FILENAME = 'U:\year5\Project\Abel.png'
root = tk.Tk()
canvas = tk.Canvas(root, width=800, height=600)
canvas.pack()
tk_img = ImageTk.PhotoImage(file = FILENAME)
canvas.image = tk_img
item = canvas.create_image(125, 125, image=tk_img)


#btn = Button(root, text = "start", command=choose_combo, activebackground = "#33B5E5")
#btn.pack()
igm = PhotoImage(file="U:\year5\Project\Start.gif")
start_button = tk.Button(root, image=igm, command = choose_combo, anchor = 'nw',
                    width = 175, activebackground = "#33B5E5")
start_button_window = canvas.create_window(450, 250, anchor='center', window=start_button)  






#photo = ImageTk.PhotoImage(Image.open("C:\Users\g00238234\Desktop\Ryu.png"));
#frame.create_image(10, 10, image = photo, anchor = SW)
#image = Image.open("C:\Users\g00238234\Desktop\Ryu.png")
#photo = ImageTk.PhotoImage(image)
#label = Label(root,image=photo,bg="yellow")
#label.image = photo # keep a reference!
#label.pack()

#btn = Button(root, text = "Menu", command=create_window)



#btn.bind('<Button-1>', click)



# keep a reference!
#frame.pack()
#label.pack()


#w.pack()
#frame.pack()







#w.start(50) #time 



	
#if length==200:
#	w.stop()


#w = Canvas(master, width=400, height = 400, bg="yellow")
#w.pack()	

#w = Scale(master, from_=0, to=100)
#w.pack()
#w = Scale(master, from_=0, to=200, orient=HORIZONTAL)







	
root.mainloop()




