from typing import Callable, Dict, Tuple

import pygame
import pygame_gui
from pygame_gui.elements import UIButton
from pygame_gui.elements import UIDropDownMenu
from pygame_gui.elements import UILabel
from pygame_gui.elements import UITextEntryLine

from protos import mobmodel_simulator_pb2


class ConfigWindow(pygame_gui.elements.UIWindow):

    CLIENT_RECT = (459, 600)

    def __init__(
        self,
        position: Tuple[int, int],
        ui_manager: pygame_gui.UIManager,
        listeners: Dict[str, Callable] = None,
    ):
        super().__init__(rect=pygame.Rect(position, ConfigWindow.CLIENT_RECT),
                         manager=ui_manager,
                         window_display_title='Mobility Model Simulator Config',
                         object_id='#config')

        w, h = ConfigWindow.CLIENT_RECT
        shrinked_w, shrinked_h = self.get_container().get_size()
        self.set_dimensions((2 * w - shrinked_w, 2 * h - shrinked_h))

        self._listeners = listeners or {}

        self._img_window = pygame_gui.elements.UIImage(
            relative_rect=pygame.Rect((0, 0), ConfigWindow.CLIENT_RECT),
            image_surface=pygame.Surface(ConfigWindow.CLIENT_RECT).convert(),
            manager=ui_manager,
            container=self,
            parent_element=self,
        )

        # yapf: disable

        ########################################################################
        # Coverage Map 설정란 시작
        ########################################################################
        position = pygame.Rect(15, 12, 300, 20)
        UILabel(position, 'Coverage Map Configuration', object_id='#cvge-map-label', manager=ui_manager, container=self)

        # ------------------------------------  Coverage Map Width, Height 설정란
        position = pygame.Rect(10, 45, 284, 50)
        self._cvge_map_size = UITextEntryLine(position, object_id='#cvge-map-size', manager=ui_manager, container=self)
        self._cvge_map_size.set_text('1000x1000')

        position = pygame.Rect(25, 37, 180, 20)
        UILabel(position, 'Map Width x Height (m)', manager=ui_manager, container=self)

        # ---------------------------------------- Coverage Map Cell Size 설정란
        position = pygame.Rect(299, 45, 150, 50)
        self._cvge_map_cell_size = UITextEntryLine(position, object_id='#cvge-map-cell-size', manager=ui_manager, container=self)
        self._cvge_map_cell_size.set_text('1.0')

        position = pygame.Rect(314, 37, 112, 20)
        UILabel(position, 'Cell Size (m)', manager=ui_manager, container=self)

        ########################################################################
        # UAV 설정란 시작
        ########################################################################
        position = pygame.Rect(15, 110, 190, 20)
        UILabel(position, 'UAV Configuration', object_id='#uav-label', manager=ui_manager, container=self)

        #----------------------------------------------------- Camera FOV 설정란
        position = pygame.Rect(10, 143, 439, 50)
        self._camera_fov = UITextEntryLine(position, object_id='#camera-fov', manager=ui_manager, container=self)
        self._camera_fov.set_text('10.0x10.0')

        position = pygame.Rect(25, 135, 225, 20)
        UILabel(position, 'Camera - Field Of View (deg)', manager=ui_manager, container=self)

        #----------------------------------------------------  UAV - Mass 설정란
        position = pygame.Rect(10, 211, 110, 50)
        self._uav_mass = UITextEntryLine(position, object_id='#uav-mass', manager=ui_manager, container=self)
        self._uav_mass.set_text('2.0')

        position = pygame.Rect(25, 203, 72, 20)
        UILabel(position, 'Mass (kg)', manager=ui_manager, container=self)

        # ---------------------------------------------- UAV - Max Thrust 설정란
        position = pygame.Rect(125, 211, 152, 50)
        self._uav_max_thrust = UITextEntryLine(position, object_id='#uav-max-thrust', manager=ui_manager, container=self)
        self._uav_max_thrust.set_text('18.5')

        position = pygame.Rect(140, 203, 117, 20)
        UILabel(position, 'Max Thrust (N)', manager=ui_manager, container=self)

        # ----------------------------------------------- UAV - Max Speed 설정란
        position = pygame.Rect(282, 211, 167, 50)
        self._uav_max_speed = UITextEntryLine(position, object_id='#uav-max-speed', manager=ui_manager, container=self)
        self._uav_max_speed.set_text('80.0')

        position = pygame.Rect(297, 203, 132, 20)
        UILabel(position, 'Max Speed (km/h)', manager=ui_manager, container=self)

        # ------------------------------------------------ UAV - Altitude 설정란
        position = pygame.Rect(10, 279, 140, 50)
        self._uav_altitude = UITextEntryLine(position, object_id='#uav-altitude', manager=ui_manager, container=self)
        self._uav_altitude.set_text('50.0')

        position = pygame.Rect(25, 271, 105, 20)
        UILabel(position, 'Altitude (m)', manager=ui_manager, container=self)

        # ----------------------------------------------  UAV - Bank Angle 설정란
        position = pygame.Rect(155, 279, 165, 50)
        self._uav_bank_angle = UITextEntryLine(position, object_id='#uav-bank-angle', manager=ui_manager, container=self)
        self._uav_bank_angle.set_text('22.5')

        position = pygame.Rect(170, 271, 130, 20)
        UILabel(position, 'Bank Angle (deg)', manager=ui_manager, container=self)

        # -------------------------------------------------- UAV - n_uavs 설정란
        position = pygame.Rect(325, 279, 124, 50)
        self._n_uavs = UITextEntryLine(position, object_id='#uavs-n', manager=ui_manager, container=self)
        self._n_uavs.set_text('10')

        position = pygame.Rect(340, 271, 40, 20)
        UILabel(position, 'UAVs', manager=ui_manager, container=self)

        # ----------------------------------------------------  UAV - type 설정란
        position = pygame.Rect(10, 340, 439, 30)
        items = ['Rotary-Wing UAV', 'Fixed-Wing UAV']
        self._uav_type = UIDropDownMenu(items, items[0], position, object_id='#uav-type', manager=ui_manager, container=self)

        ########################################################################
        # Mobility Model 설정란 시작
        ########################################################################
        position = pygame.Rect(15, 385, 300, 20)
        UILabel(position, 'Mobility Model Configuration', object_id='#uav-label', manager=ui_manager, container=self)

        # ----------------------------------------- Mobility Model - type 설정란
        position = pygame.Rect(10, 410, 439, 30)
        items = ['Random Waypoint', 'RDPZ']
        self._mobmodel = UIDropDownMenu(items, items[0], position, object_id='#uav-type', manager=ui_manager, container=self)

        ########################################################################
        # Mobility Model Simulator 제어란 시작
        ########################################################################

        # -------------------------------- Mobility Model - Start Simulation 버튼
        position = pygame.Rect(10, 450, 200, 50)
        self._start_simul = UIButton(position, 'Start Simulation', object_id='#btn-start-simul', manager=self.ui_manager, container=self)

        # --------------------------------- Mobility Model - Stop Simulation 버튼
        position = pygame.Rect(249, 450, 200, 50)
        self._stop_simul = UIButton(position, 'Stop Simulation', object_id='#btn-stop-simul', manager=self.ui_manager, container=self)

        # yapf: enable

    def process_event(self, event: pygame.event.Event) -> bool:
        consumed_event = False
        if hasattr(event, 'ui_element') and event.ui_element == self.close_window_button:
            consumed_event = True
        else:
            if event.type == pygame_gui.UI_BUTTON_PRESSED:
                last_object_id = event.ui_element.object_ids[-1]
                if last_object_id  == '#btn-start-simul':
                    self._invoke_callback(last_object_id, self.cfg)
                    consumed_event = True
                elif last_object_id == '#btn-stop-simul':
                    self._invoke_callback(last_object_id)
                    consumed_event = True

        return (consumed_event or super().process_event(event))

    @property
    def cfg(self) -> mobmodel_simulator_pb2.MobilityModelSimulatorConfig:
        # pylint: disable=no-member

        cfg_ = mobmodel_simulator_pb2.MobilityModelSimulatorConfig()

        if self._mobmodel.selected_option not in ['Random Waypoint', 'RDPZ']:
            cfg_.mobmodel = mobmodel_simulator_pb2.UNKNOWN_MOBILITY_MODEL
        elif self._mobmodel.selected_option == 'Random Waypoint':
            cfg_.mobmodel = mobmodel_simulator_pb2.RANDOM_WAYPOINT
        elif self._mobmodel.selected_option == 'RDPZ':
            cfg_.mobmodel = mobmodel_simulator_pb2.UNKNOWN_MOBILITY_MODEL

        map_width, map_height = map(float, self._cvge_map_size.text.split('x'))
        cfg_.coverage_map_cfg.map_width = map_width
        cfg_.coverage_map_cfg.map_height = map_height
        cfg_.coverage_map_cfg.cell_size = float(self._cvge_map_cell_size.text)

        hfov, vfov = map(float, self._camera_fov.text.split('x'))
        cfg_.uav_cfg.camera_cfg.hfov = hfov
        cfg_.uav_cfg.camera_cfg.vfov = vfov
        cfg_.uav_cfg.mass = float(self._uav_mass.text)
        cfg_.uav_cfg.max_thrust = float(self._uav_max_thrust.text)
        cfg_.uav_cfg.max_speed = float(self._uav_max_speed.text)
        cfg_.uav_cfg.altitude = float(self._uav_altitude.text)
        cfg_.uav_cfg.bank_angle = float(self._uav_bank_angle.text)
        if self._uav_type.selected_option not in ['Rotary-Wing UAV', 'Fixed-Wing UAV']:
            cfg_.uav_cfg.uav_type = mobmodel_simulator_pb2.UNKNOWN_UAV_TYPE
        elif self._uav_type.selected_option == 'Rotary-Wing UAV':
            cfg_.uav_cfg.uav_type = mobmodel_simulator_pb2.ROTARY_WING
        elif self._uav_type == 'Fixed-Wing UAV':
            cfg_.uav_cfg.uav_type = mobmodel_simulator_pb2.FIXED_WING

        cfg_.n_uavs = int(self._n_uavs.text)

        return cfg_

    def _invoke_callback(self, object_id: str, *args, **kargs):
        if object_id in self._listeners:
            self._listeners[object_id](*args, **kargs)
