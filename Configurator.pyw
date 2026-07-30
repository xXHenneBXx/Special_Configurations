#!/usr/bin/env python3
# ------------------------------------------------------------------------------
# Pro FW Configurator
# Generates Marlin configuration files for V2.1.2.8
# Created by xXHenneBXx
# Professional Firmware Configurator + ProUI by MRiscoC
# URL: https://github.com/xXHenneBXx/Special_Configurations
# version: 1.0
# date: 2026/07/25 
# ------------------------------------------------------------------------------

import os
import tkinter as tk
from tkinter import END, messagebox, ttk
import CreateConfigs

ConfigList = []

# ─── Color Palette ───────────────────────────────────────────────────────────
COLOR_BG          = "#0d1117"
COLOR_PANEL       = "#161b22"
COLOR_PANEL_LIGHT = "#1c2128"
COLOR_BORDER      = "#30363d"
COLOR_TEXT        = "#c9d1d9"
COLOR_TEXT_DIM    = "#8b949e"
COLOR_ACCENT      = "#58a6ff"
COLOR_ACCENT_HOVER= "#79c0ff"
COLOR_SUCCESS     = "#3fb950"
COLOR_WARNING     = "#d29922"
COLOR_ERROR       = "#f85149"
COLOR_DISABLED    = "#484f58"


class ModernStyle:
    """Applies a modern dark theme to ttk widgets."""

    @staticmethod
    def apply(style):
        style.theme_use("clam")

        # Base widgets
        style.configure(".", background=COLOR_BG, foreground=COLOR_TEXT,
                        font=("Segoe UI", 10))
        style.configure("TFrame", background=COLOR_BG)
        style.configure("Panel.TFrame", background=COLOR_PANEL)
        style.configure("PanelLight.TFrame", background=COLOR_PANEL_LIGHT)

        # Labels
        style.configure("TLabel", background=COLOR_BG, foreground=COLOR_TEXT,
                        font=("Segoe UI", 10))
        style.configure("Title.TLabel", background=COLOR_BG, foreground=COLOR_ACCENT,
                        font=("Segoe UI Semibold", 16))
        style.configure("Subtitle.TLabel", background=COLOR_BG, foreground=COLOR_TEXT_DIM,
                        font=("Segoe UI", 9))
        style.configure("Section.TLabel", background=COLOR_PANEL, foreground=COLOR_ACCENT,
                        font=("Segoe UI Semibold", 11))
        style.configure("Panel.TLabel", background=COLOR_PANEL, foreground=COLOR_TEXT,
                        font=("Segoe UI", 10))
        style.configure("PanelDim.TLabel", background=COLOR_PANEL, foreground=COLOR_TEXT_DIM,
                        font=("Segoe UI", 9))

        # Buttons
        style.configure("TButton", background=COLOR_PANEL_LIGHT, foreground=COLOR_TEXT,
                        borderwidth=0, focuscolor=COLOR_ACCENT, padding=(16, 8),
                        font=("Segoe UI", 10))
        style.map("TButton",
                  background=[("active", COLOR_BORDER), ("pressed", COLOR_ACCENT)],
                  foreground=[("active", COLOR_ACCENT_HOVER)])
        style.configure("Accent.TButton", background=COLOR_ACCENT, foreground="#ffffff",
                        padding=(20, 10), font=("Segoe UI Semibold", 10))
        style.map("Accent.TButton",
                  background=[("active", COLOR_ACCENT_HOVER), ("pressed", COLOR_ACCENT)],
                  foreground=[("active", "#ffffff")])
        style.configure("Success.TButton", background=COLOR_SUCCESS, foreground="#ffffff",
                        padding=(20, 10), font=("Segoe UI Semibold", 10))
        style.map("Success.TButton",
                  background=[("active", "#46d369"), ("pressed", COLOR_SUCCESS)])

        # Radio buttons
        style.configure("TRadiobutton", background=COLOR_PANEL, foreground=COLOR_TEXT,
                        font=("Segoe UI", 10))
        style.map("TRadiobutton",
                  background=[("active", COLOR_PANEL_LIGHT)],
                  foreground=[("active", COLOR_ACCENT_HOVER), ("selected", COLOR_ACCENT)])

        # Checkbuttons
        style.configure("TCheckbutton", background=COLOR_PANEL, foreground=COLOR_TEXT,
                        font=("Segoe UI", 10))
        style.map("TCheckbutton",
                  background=[("active", COLOR_PANEL_LIGHT)],
                  foreground=[("active", COLOR_ACCENT_HOVER), ("selected", COLOR_ACCENT)])

        # Entry
        style.configure("TEntry", fieldbackground=COLOR_PANEL_LIGHT, foreground=COLOR_TEXT,
                        borderwidth=1, relief="solid", padding=6)
        style.map("TEntry", fieldbackground=[("focus", COLOR_PANEL)])

        # Combobox (unused but styled for consistency)
        style.configure("TCombobox", fieldbackground=COLOR_PANEL_LIGHT, foreground=COLOR_TEXT,
                        background=COLOR_PANEL_LIGHT)

        # Separator
        style.configure("TSeparator", background=COLOR_BORDER)


def fill_conf(obj):
    global ConfigList
    ConfigList.clear()

    if obj.printer.get():
        ConfigList.append(obj.printer.get())
    if obj.board.get():
        ConfigList.append(obj.board.get())
    if obj.leveling.get():
        ConfigList.append(obj.leveling.get())
    if obj.ubl.get():
        ConfigList.append("UBL")
    if not obj.display.get() == "DWIN":
        ConfigList.append(obj.display.get())
    if not obj.thermistor.get() == "T1":
        ConfigList.append(obj.thermistor.get())
    
    for checkbox in obj.featurelist:
        if checkbox.instate(["selected"]):
            ConfigList.append(checkbox.cget("text"))


def set_conf():
    fill_conf(root)
    root.update_conf()


def generate_conf():
    global ConfigList
    fill_conf(root)
    root.update_conf()
    error = CreateConfigs.Generate(root.ConfigName.get(), ConfigList)
    if error:
        root.open_log()
    else:
        messagebox.showinfo(
            message="Configuration files generated successfully.",
            title="Pro FW",
        )


def copy_clpbrd():
    root.clipboard_clear()
    root.clipboard_append(root.Edit_GenFunc.get(1.0, END))
    messagebox.showinfo(
        message="Configuration generator command copied to clipboard.",
        title="Clipboard",
    )


def auto_name():
    fill_conf(root)
    name = "-".join(ConfigList)
   #filter S1 printer name
    Name = Name.replace("-301F","-F").replace("F1-BLT","F1").replace("F4-BLT","F4")
    #filter UBL
    if "F1-UBL" not in Name and "F4-UBL" not in Name:
      Name = Name.replace("-UBL","UBL")
    #rename LinaAdv
    #Name = Name.replace("LinAdv","LA") 
    root.ConfigName.delete(0, END)
    root.ConfigName.insert(0, name)
    root.update_conf()


class Main(tk.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.configure(width=960, height=620, bg=COLOR_BG)
        self.resizable(False, False)
        self.title("Pro FW Configurator")

        # Apply modern style
        self.style = ttk.Style(self)
        ModernStyle.apply(self.style)

        # ─── Header ──────────────────────────────────────────────────────
        self.header = ttk.Frame(self, style="Panel.TFrame")
        self.header.pack(fill="x", padx=0, pady=0)
        self.header.configure(height=70)

        self.title_label = ttk.Label(
            self.header, text="Advanced Pro Firmware Configurator",
            style="Title.TLabel"
        )
        # Override bg for header label
        self.style.configure("Title.TLabel", background=COLOR_PANEL)
        self.title_label.place(x=24, y=12)

        self.subtitle_label = ttk.Label(
            self.header,
            text="Marlin firmware configurations With Additional Pulltrusion Features",
            style="PanelDim.TLabel"
        )
        self.subtitle_label.place(x=24, y=40)

        # Accent bar at bottom of header
        self.accent_bar = tk.Frame(self.header, bg=COLOR_ACCENT, height=2)
        self.accent_bar.pack(side="bottom", fill="x")

        # ─── Main content area ───────────────────────────────────────────
        self.content = ttk.Frame(self)
        self.content.pack(fill="both", expand=True, padx=16, pady=12)

        # ─── Config name row ─────────────────────────────────────────────
        self.name_frame = ttk.Frame(self.content)
        self.name_frame.pack(fill="x", pady=(0, 10))

        self.l1 = ttk.Label(self.name_frame, text="Configuration Name:")
        self.l1.pack(side="left", padx=(0, 8))

        self.ConfigName = ttk.Entry(self.name_frame, width=55)
        self.ConfigName.insert(0, "MyConfig")
        self.ConfigName.pack(side="left", padx=(0, 8), fill="x", expand=True)

        self.boton1 = ttk.Button(self.name_frame, text="Auto Name", command=auto_name)
        self.boton1.pack(side="left")

        # ─── Selection panels ────────────────────────────────────────────
        self.panels_frame = ttk.Frame(self.content)
        self.panels_frame.pack(fill="both", expand=True, pady=(0, 10))

        # Create a canvas + scrollbar for the panels area
        self.canvas = tk.Canvas(self.panels_frame, bg=COLOR_BG,
                                highlightthickness=0, bd=0)
        self.scrollbar = ttk.Scrollbar(self.panels_frame, orient="vertical",
                                       command=self.canvas.yview)
        self.scrollable = ttk.Frame(self.canvas)

        self.scrollable.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )
        self.canvas.create_window((0, 0), window=self.scrollable, anchor="nw")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        self.canvas.pack(side="left", fill="both", expand=True)
        self.scrollbar.pack(side="right", fill="y")

        # Bind mouse wheel
        self.canvas.bind_all("<MouseWheel>",
                             lambda e: self.canvas.yview_scroll(int(-1*(e.delta/120)), "units"))

        # Build selection columns
        self._build_printer_col(self.scrollable, 0)
        self._build_board_col(self.scrollable, 1)
        self._build_leveling_col(self.scrollable, 2)
        self._build_display_col(self.scrollable, 3)
        self._build_thermistor_col(self.scrollable, 4)
        self._build_features_col(self.scrollable, 5)

        # ─── Bottom action area ──────────────────────────────────────────
        self.bottom = ttk.Frame(self.content)
        self.bottom.pack(fill="x", pady=(8, 0))

        # Action buttons
        self.btn_frame = ttk.Frame(self.bottom)
        self.btn_frame.pack(fill="x", pady=(0, 8))

        self.boton2 = ttk.Button(self.btn_frame, text="Set Config",
                                 command=set_conf)
        self.boton2.pack(side="left", padx=(0, 8))

        self.boton3 = ttk.Button(self.btn_frame, text="Generate",
                                 style="Success.TButton",
                                 command=generate_conf)
        self.boton3.pack(side="left", padx=(0, 8))

        self.boton4 = ttk.Button(self.btn_frame, text="Copy to Clipboard",
                                 command=copy_clpbrd)
        self.boton4.pack(side="left")

        # Generated command display
        self.cmd_label = ttk.Label(self.bottom, text="Generated Command:",
                                   style="Subtitle.TLabel")
        self.cmd_label.pack(anchor="w", pady=(4, 2))

        self.Edit_GenFunc = tk.Text(
            self.bottom, width=110, height=3, relief="flat",
            bg=COLOR_PANEL_LIGHT, fg=COLOR_SUCCESS,
            font=("Consolas", 10), padx=10, pady=8,
            borderwidth=1, highlightthickness=1,
            highlightbackground=COLOR_BORDER, highlightcolor=COLOR_ACCENT
        )
        self.Edit_GenFunc.pack(fill="x", pady=(0, 0))

        # Initialize
        fill_conf(self)
        self.update_conf()

    def _make_panel(self, parent, col, title):
        """Create a titled panel column inside the scrollable area."""
        panel = ttk.Frame(parent, style="Panel.TFrame", padding=12)
        panel.grid(row=0, column=col, padx=6, pady=0, sticky="nw")

        lbl = ttk.Label(panel, text=title, style="Section.TLabel")
        lbl.pack(anchor="w", pady=(0, 8))

        # Accent underline
        bar = tk.Frame(panel, bg=COLOR_ACCENT, height=1)
        bar.pack(fill="x", pady=(0, 8))

        inner = ttk.Frame(panel, style="Panel.TFrame")
        inner.pack(anchor="w", fill="x")
        return inner

    def _build_printer_col(self, parent, col):
        inner = self._make_panel(parent, col, "PRINTER")

        self.printer = tk.StringVar(self, "RecreatorMK3")
        self.printerlist = []
        for file in sorted(os.listdir("_printers")):
            if file.endswith(".json") and not file.endswith("Common.json"):
                value = file.replace(".json", "")
                rb = ttk.Radiobutton(inner, text=value, variable=self.printer,
                                     value=value, command=set_conf)
                rb.pack(anchor="w", pady=1)

    def _build_board_col(self, parent, col):
        inner = self._make_panel(parent, col, "BOARD")

        self.board = tk.StringVar(self, "422")
        for file in sorted(os.listdir("_boards")):
            if file.endswith(".json"):
                value = file.replace(".json", "")
                rb = ttk.Radiobutton(inner, text=value, variable=self.board,
                                     value=value, command=set_conf)
                rb.pack(anchor="w", pady=1)

    def _build_leveling_col(self, parent, col):
        inner = self._make_panel(parent, col, "LEVELING")

        self.leveling = tk.StringVar(self, "BLT")
        self.levelinglist = []
        for file in sorted(os.listdir("_leveling")):
            if file.endswith(".json") and not file.endswith("UBL.json"):
                value = file.replace(".json", "")
                rb = ttk.Radiobutton(inner, text=value, variable=self.leveling,
                                     value=value, command=self.set_ubl)
                rb.pack(anchor="w", pady=1)
                self.levelinglist.append(rb)

        # UBL is a dependent checkbox, not a leveling method itself -
        # it's disabled automatically for manual mesh ("MM") leveling
        self.ubl = tk.BooleanVar(self, True)
        self.ublchkb = ttk.Checkbutton(inner, text="UBL", variable=self.ubl,
                                       command=set_conf)
        self.ublchkb.pack(anchor="w", pady=(6, 1))

    def _build_display_col(self, parent, col):
        inner = self._make_panel(parent, col, "DISPLAY")

        self.display = tk.StringVar(self, "DWIN")
        for file in sorted(os.listdir("_displays")):
            if file.endswith(".json"):
                value = file.replace(".json", "")
                rb = ttk.Radiobutton(inner, text=value, variable=self.display,
                                     value=value, command=set_conf)
                rb.pack(anchor="w", pady=1)

    def _build_thermistor_col(self, parent, col):
        inner = self._make_panel(parent, col, "THERMISTOR")

        self.thermistor = tk.StringVar(self, "T1")
        for file in sorted(os.listdir("_thermistor")):
            if file.endswith(".json"):
                value = file.replace(".json", "")
                rb = ttk.Radiobutton(inner, text=value, variable=self.thermistor,
                                     value=value, command=set_conf)
                rb.pack(anchor="w", pady=1)

    def _build_features_col(self, parent, col):
        inner = self._make_panel(parent, col, "FEATURES")

        self.featurelist = []
        for file in sorted(os.listdir("_features")):
            if file.endswith(".json"):
                cb = ttk.Checkbutton(inner, text=file.replace(".json", ""),
                                     command=set_conf)
                cb.pack(anchor="w", pady=1)
                cb.state(["!alternate"])
                self.featurelist.append(cb)

    def update_conf(self):
        name = self.ConfigName.get()
        self.Edit_GenFunc.delete(1.0, END)
        self.Edit_GenFunc.insert(END, f"CreateConfigs.Generate('{name}',[")
        self.Edit_GenFunc.insert(END, ",".join(f"'{item}'" for item in ConfigList))
        self.Edit_GenFunc.insert(END, "])")
                                                 
    def set_ubl(self):
        if self.leveling.get() == "MM":
            self.ubl.set(False)
            self.ublchkb.configure(state="disabled")
        else:
            self.ublchkb.configure(state="enabled")
        set_conf()

    def open_log(self):
        try:
            with open(self.ConfigName.get() + "/log.txt", "r") as f:
                loglines = f.read()
            d = log_window(self)
            d.LogText.delete(1.0, END)
            d.LogText.insert(END, loglines)
        except Exception as e:
            messagebox.showinfo(message=str(e), title="Error")


class log_window(tk.Toplevel):
    def __init__(self, parent):
        super().__init__(parent)
        self.geometry("600x400")
        self.resizable(True, True)
        self.title("Error Log")
        self.configure(bg=COLOR_BG)

        # Style the log window
        header = ttk.Frame(self, style="Panel.TFrame")
        header.pack(fill="x")
        ttk.Label(header, text="Build Log / Errors",
                  style="Section.TLabel").pack(padx=12, pady=8)

        self.LogText = tk.Text(
            self, relief="flat", bg=COLOR_PANEL_LIGHT, fg=COLOR_ERROR,
            font=("Consolas", 10), padx=12, pady=10,
            borderwidth=1, highlightthickness=1,
            highlightbackground=COLOR_BORDER
        )
        self.LogText.pack(fill="both", expand=True, padx=12, pady=(0, 8))

        btn_frame = ttk.Frame(self)
        btn_frame.pack(fill="x", padx=12, pady=(0, 12))
        ttk.Button(btn_frame, text="Close", style="Accent.TButton",
                   command=self.destroy).pack()
        self.grab_set()


if __name__ == "__main__":
    root = Main()
    root.mainloop()
