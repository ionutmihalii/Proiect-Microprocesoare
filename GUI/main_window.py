import serial
from PySide6.QtCore import QTimer
from PySide6.QtWidgets import (
    QMainWindow,
    QHBoxLayout,
    QVBoxLayout,
    QWidget,
    QGroupBox,
    QLabel,
    QPushButton,
    QTextEdit,
)
from PySide6.QtGui import QIcon, QFont
import pyqtgraph as pg
from PySide6.QtCore import Qt


class MainWindow(QMainWindow):
    first: int = 0
    direction: int = 0
    aux: int = 0
    promotie: str = "2023-2024"
    team: list[str] = [
        "MIHALI SORIN-IONUT",
        "POPESCU VLAD",
    ]

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))

        self.serial_port = serial.Serial(port="COM14", baudrate=115200)
        self.heights = [0, 0, 0]

        primary_layout = QVBoxLayout()
        secondary_layout = QHBoxLayout()
        tertiary_layout = QVBoxLayout()

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member, 1)
        team_box_layout.addWidget(second_member, 1)
        team_box.setLayout(team_box_layout)

        control_panel_box = QGroupBox("Control Panel")
        control_panel_box.setFont(bold_font)

        button1 = QPushButton("Reverse LED's Color")
        control_panel_box_layout = QVBoxLayout()
        button1.clicked.connect(self.send_data_to_microcontroller)
        control_panel_box_layout.addWidget(button1, 1)
        self.text = QLabel("----->")
        self.text.setFont(bold_font)
        self.text.setAlignment(Qt.AlignCenter)
        control_panel_box_layout.addWidget(self.text, 2)
        control_panel_box.setLayout(control_panel_box_layout)

        tertiary_layout.addWidget(team_box, 1)
        tertiary_layout.addWidget(control_panel_box, 5)

        self.plot_widget = pg.PlotWidget()
        interval = [1.1, 2.2, 3.3]

        bar_plot = pg.BarGraphItem(
            x=interval, height=self.heights, width=0.6, brush="b"
        )
        self.plot_widget.addItem(bar_plot)

        secondary_layout.addWidget(self.plot_widget, 3)
        secondary_layout.addLayout(tertiary_layout, 1)

        primary_layout.addLayout(secondary_layout, 4)
        self.text_edit = QTextEdit()
        self.text_edit.setReadOnly(True)

        debug_box = QGroupBox("Logs")
        debug_box_layout = QVBoxLayout()
        debug_box_layout.addWidget(self.text_edit, 1)
        debug_box.setLayout(debug_box_layout)

        primary_layout.addWidget(debug_box, 1)

        widget = QWidget()
        widget.setLayout(primary_layout)
        self.setCentralWidget(widget)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.check_serial_data)
        self.timer.start(1)

    def check_serial_data(self):
        if self.serial_port.in_waiting:
            received_data = self.serial_port.readline().decode("utf-8").strip()

            try:
                if self.first == 0:
                    self.first = 1
                else:
                    received_value = float(received_data)
                    self.text_edit.append(f"Received voltage: {received_value}")
                    self.update_plot(received_value)

            except ValueError:
                self.aux = 1

    def send_data_to_microcontroller(self):
        self.serial_port.write(b"f")
        self.text_edit.append(f"Switching LED's color")

        if self.direction == 0:
            self.direction = 1
            self.text.setText("<-----")
        else:
            self.direction = 0
            self.text.setText("----->")

    def update_plot(self, received_value):
        if received_value <= 1.1:
            self.heights[0] = self.heights[0] + 1
        elif received_value <= 2.2:
            self.heights[1] = self.heights[1] + 1
        elif received_value <= 3.3:
            self.heights[2] = self.heights[2] + 1

        brushes = ["g", "y", "r"]
        interval = [1.1, 2.2, 3.3]
        bar_plots = []
        for i, (x_val, height_val) in enumerate(zip(interval, self.heights)):
            bar_plot = pg.BarGraphItem(
                x=[x_val], height=[height_val], width=0.6, brush=brushes[i]
            )
            bar_plots.append(bar_plot)

        self.plot_widget.clear()
        for bar_plot in bar_plots:
            self.plot_widget.addItem(bar_plot)

    def closeEvent(self, event):
        if self.serial_port.is_open:
            self.serial_port.close()
        event.accept()
