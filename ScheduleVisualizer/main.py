import tkinter as tk
from tkinter import ttk, messagebox
import json
import os
from datetime import datetime

# Constants
DAYS = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
START_HOUR = 8
END_HOUR = 23  # 11 PM
INTERVAL_MINUTES = 30
DATA_FILE = "schedule_data.json"

class ScheduleApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Team Availability Visualizer")
        self.root.geometry("1000x800")

        # Data storage: {Name: {Day: [ (start_tuple, end_tuple) ] }}
        # Start/End tuple: (hour, minute) in 24h format
        self.schedule_data = {}
        self.load_data()

        self.setup_ui()

    def setup_ui(self):
        # Main Layout
        main_pane = ttk.PanedWindow(self.root, orient=tk.HORIZONTAL)
        main_pane.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # Left Panel: Input
        input_frame = ttk.LabelFrame(main_pane, text="Input Availability")
        main_pane.add(input_frame, weight=1)

        self.create_input_widgets(input_frame)

        # Right Panel: Visualization
        vis_frame = ttk.LabelFrame(main_pane, text="Schedule Visualization")
        main_pane.add(vis_frame, weight=4)

        self.create_grid_widgets(vis_frame)

    def create_input_widgets(self, parent):
        # Name Input
        ttk.Label(parent, text="Name:").pack(pady=(10, 0), padx=5, anchor="w")
        self.name_var = tk.StringVar()
        self.name_entry = ttk.Entry(parent, textvariable=self.name_var)
        self.name_entry.pack(pady=5, padx=5, fill=tk.X)

        # Days Selection
        ttk.Label(parent, text="Select Day:").pack(pady=(10, 0), padx=5, anchor="w")
        self.day_var = tk.StringVar(value=DAYS[0])
        self.day_combo = ttk.Combobox(parent, textvariable=self.day_var, values=DAYS, state="readonly")
        self.day_combo.pack(pady=5, padx=5, fill=tk.X)

        # Time Selection
        time_frame = ttk.Frame(parent)
        time_frame.pack(pady=10, padx=5, fill=tk.X)

        times = self.generate_time_labels()
        
        ttk.Label(time_frame, text="Start Time:").grid(row=0, column=0, sticky="w")
        self.start_time_var = tk.StringVar(value=times[0])
        self.start_time_combo = ttk.Combobox(time_frame, textvariable=self.start_time_var, values=times, state="readonly")
        self.start_time_combo.grid(row=1, column=0, sticky="ew", padx=(0, 5))

        ttk.Label(time_frame, text="End Time:").grid(row=0, column=1, sticky="w")
        self.end_time_var = tk.StringVar(value=times[-1])
        self.end_time_combo = ttk.Combobox(time_frame, textvariable=self.end_time_var, values=times, state="readonly")
        self.end_time_combo.grid(row=1, column=1, sticky="ew")
        
        time_frame.columnconfigure(0, weight=1)
        time_frame.columnconfigure(1, weight=1)

        # Buttons
        ttk.Button(parent, text="Add Availability", command=self.add_availability).pack(pady=20, padx=5, fill=tk.X)
        ttk.Button(parent, text="Clear User's Data", command=self.clear_user).pack(pady=5, padx=5, fill=tk.X)
        ttk.Button(parent, text="Clear All Data", command=self.clear_all).pack(pady=5, padx=5, fill=tk.X)
        
        # Legend/Info
        info_lbl = ttk.Label(parent, text="Tip: Darker colors indicate\nmore people available.\nHover to see full list.", foreground="gray")
        info_lbl.pack(pady=20, padx=5)

    def create_grid_widgets(self, parent):
        # Scrollable Canvas
        self.canvas = tk.Canvas(parent, bg="white")
        h_scroll = ttk.Scrollbar(parent, orient="horizontal", command=self.canvas.xview)
        v_scroll = ttk.Scrollbar(parent, orient="vertical", command=self.canvas.yview)
        
        self.canvas.configure(xscrollcommand=h_scroll.set, yscrollcommand=v_scroll.set)
        
        h_scroll.pack(side=tk.BOTTOM, fill=tk.X)
        v_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        self.grid_frame = tk.Frame(self.canvas, bg="white")
        self.canvas.create_window((0, 0), window=self.grid_frame, anchor="nw")
        
        self.grid_frame.bind("<Configure>", lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all")))

        self.draw_grid()

    def format_time_ampm(self, h, m):
        d = datetime(2000, 1, 1, h, m)
        return d.strftime("%I:%M %p").lstrip("0")

    def parse_time_ampm(self, time_str):
        # Parses "8:30 AM" back to (8, 30) (24h)
        # Using a dummy date
        try:
            d = datetime.strptime(time_str, "%I:%M %p")
            return d.hour, d.minute
        except ValueError:
            # Fallback for simple "8:30" if ever used
            d = datetime.strptime(time_str, "%H:%M")
            return d.hour, d.minute

    def generate_time_labels(self):
        labels = []
        for h in range(START_HOUR, END_HOUR + 1):
            for m in (0, 30):
                if h == END_HOUR and m == 30: continue
                labels.append(self.format_time_ampm(h, m))
        
        # Add the closing 11:00 PM (or 11:30 PM if needed as end of slot, but logic cuts off at end)
        # To allow scheduling UNTIL 11:00 PM, we might need 11:00 PM as an end option.
        labels.append(self.format_time_ampm(END_HOUR, 0)) # 11:00 PM
        if END_HOUR == 23:
             # If 11pm is the limit, 11:30 isn't available. 
             pass
        return labels

    def draw_grid(self):
        # Clear existing
        for widget in self.grid_frame.winfo_children():
            widget.destroy()

        # Headers (Days)
        tk.Label(self.grid_frame, text="Time / Day", borderwidth=1, relief="solid", bg="lightgray", width=12).grid(row=0, column=0, sticky="nsew")
        for i, day in enumerate(DAYS):
            tk.Label(self.grid_frame, text=day, borderwidth=1, relief="solid", bg="lightgray", width=20).grid(row=0, column=i+1, sticky="nsew")

        # Time Slots (Rows)
        current_h, current_m = START_HOUR, 0
        row = 1
        
        while current_h < END_HOUR or (current_h == END_HOUR and current_m == 0):
            time_str = self.format_time_ampm(current_h, current_m)
            
            tk.Label(self.grid_frame, text=time_str, borderwidth=1, relief="solid", bg="#f0f0f0").grid(row=row, column=0, sticky="nsew")
            
            # Create cells for each day
            for col, day in enumerate(DAYS):
                available_people = self.get_people_for_slot(day, current_h, current_m)
                
                cell_bg = "white"
                
                count = len(available_people)
                cell_text = ""
                
                if count > 0:
                    # Heatmap logic
                    green_val = 255
                    red_blue_val = max(255 - (count * 40), 100)
                    cell_bg = f"#{red_blue_val:02x}{green_val:02x}{red_blue_val:02x}"
                    
                    # Display names separated by comma
                    cell_text = ", ".join(available_people)
                
                # Use a larger height or width if needed, or wrap length
                lbl = tk.Label(self.grid_frame, text=cell_text, bg=cell_bg, borderwidth=1, relief="solid", height=2, wraplength=140, justify="center")
                lbl.grid(row=row, column=col+1, sticky="nsew")
                
                if count > 0:
                    lbl.bind("<Button-1>", lambda e, p=available_people, d=day, t=time_str: self.show_details(d, t, p))

            # Increment
            current_m += INTERVAL_MINUTES
            if current_m >= 60:
                current_m = 0
                current_h += 1
            
            row += 1

    def get_people_for_slot(self, day, h, m):
        people = []
        slot_time_val = h * 60 + m
        
        for name, sched in self.schedule_data.items():
            if day in sched:
                for (start_h, start_m), (end_h, end_m) in sched[day]:
                    start_val = start_h * 60 + start_m
                    end_val = end_h * 60 + end_m
                    
                    if start_val <= slot_time_val < end_val:
                        people.append(name)
        return sorted(people) # Sort names for consistency

    def add_availability(self):
        name = self.name_var.get().strip()
        if not name:
            messagebox.showerror("Error", "Please enter a name.")
            return

        day = self.day_var.get()
        start = self.start_time_var.get()
        end = self.end_time_var.get()
        
        try:
            sh, sm = self.parse_time_ampm(start)
            eh, em = self.parse_time_ampm(end)
        except Exception:
            messagebox.showerror("Error", "Invalid time format.")
            return

        if (sh * 60 + sm) >= (eh * 60 + em):
            messagebox.showerror("Error", "Start time must be before end time.")
            return

        if name not in self.schedule_data:
            self.schedule_data[name] = {}
        if day not in self.schedule_data[name]:
            self.schedule_data[name][day] = []
            
        self.schedule_data[name][day].append(((sh, sm), (eh, em)))
        self.save_data()
        self.draw_grid()
        
        messagebox.showinfo("Success", f"Added availability for {name} on {day}.")

    def clear_user(self):
        name = self.name_var.get().strip()
        if name in self.schedule_data:
            del self.schedule_data[name]
            self.save_data()
            self.draw_grid()
            messagebox.showinfo("Success", f"Cleared data for {name}.")
        else:
            messagebox.showwarning("Warning", "User not found.")

    def clear_all(self):
        if messagebox.askyesno("Confirm", "Are you sure you want to clear ALL data?"):
            self.schedule_data = {}
            self.save_data()
            self.draw_grid()

    def show_details(self, day, time, people):
        people_str = "\n".join(people)
        messagebox.showinfo(f"Details: {day} @ {time}", f"Available:\n{people_str}")

    def save_data(self):
        with open(DATA_FILE, "w") as f:
            json.dump(self.schedule_data, f, indent=4)

    def load_data(self):
        if os.path.exists(DATA_FILE):
            try:
                with open(DATA_FILE, "r") as f:
                    self.schedule_data = json.load(f)
            except Exception as e:
                print(f"Error loading data: {e}")
                self.schedule_data = {}

if __name__ == "__main__":
    root = tk.Tk()
    app = ScheduleApp(root)
    root.mainloop()
