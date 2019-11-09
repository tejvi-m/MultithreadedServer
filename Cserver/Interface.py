import socket
import time
import tkinter as tk

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('localhost', 5008))
#
# time.sleep(0.2)
# print ('Received message:', client.recv(1024))


def show_entry_fields():
    client.send(e1.get().encode())
    print("File requested. will be copied to the current directory under the same name.")
    T = tk.Text(master, height=2, width=30)
    T.grid(row=5, column = 0)
    T.insert(tk.END, client.recv(1024).decode())
    # print ('Received message:', )

master = tk.Tk()
tk.Label(master,
         text="First Name").grid(row=0)

e1 = tk.Entry(master)

e1.grid(row=0, column=1)

tk.Button(master,
          text='Request', command=show_entry_fields).grid(row=3,
                                                       column=1,
                                                       sticky=tk.W,
                                                       pady=4)

tk.mainloop()
