// Copyright � 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Colored.h"

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		static Map<std::string_view, ThemeColor> CustomColorInfo =
		{
			{ "rDefault", ThemeColor_Default },
			{ "rListView_BkColor", ThemeColor_ListView_Color },
			{ "rTreeView_BkColor", ThemeColor_TreeView_Color },
			{ "rEdit_BkColor", ThemeColor_Edit_Color },
			{ "rEdit_BkColor_Disabled", ThemeColor_Edit_Color_Disabled },
			{ "rText_1", ThemeColor_Text_1 },
			{ "rText_2", ThemeColor_Text_2 },
			{ "rText_3", ThemeColor_Text_3 },
			{ "rText_4", ThemeColor_Text_4 },
			{ "rMDIWindow", ThemeColor_MDIWindow },
			{ "rDefault_Gradient_Start", ThemeColor_Default_Gradient_Start },
			{ "rDefault_Gradient_End", ThemeColor_Default_Gradient_End },
			{ "rDivider_Highlighter_Disabled_Gradient_Start", ThemeColor_Divider_Highlighter_Disabled_Gradient_Start },
			{ "rDivider_Highlighter_Disabled_Gradient_End,", ThemeColor_Divider_Highlighter_Disabled_Gradient_End },
			{ "rDivider_Color", ThemeColor_Divider_Color },
			{ "rDivider_Color_Disabled", ThemeColor_Divider_Color_Disabled },
			{ "rDivider_Highlighter", ThemeColor_Divider_Highlighter },
			{ "rDivider_Highlighter_Hot", ThemeColor_Divider_Highlighter_Hot },
			{ "rDivider_Color_Ver2", ThemeColor_Divider_Color_Ver2 },
			{ "rDivider_Color_Disabled_Ver2", ThemeColor_Divider_Color_Disabled_Ver2 },
			{ "rDivider_Highlighter_Ver2", ThemeColor_Divider_Highlighter_Ver2 },
			{ "rDivider_Highlighter_Disabled_Ver2", ThemeColor_Divider_Highlighter_Disabled_Ver2 },
			{ "rDivider_Highlighter_Gradient_Start", ThemeColor_Divider_Highlighter_Gradient_Start },
			{ "rDivider_Highlighter_Gradient_End", ThemeColor_Divider_Highlighter_Gradient_End },
			{ "rDivider_Highlighter_Hot_Gradient_Start", ThemeColor_Divider_Highlighter_Hot_Gradient_Start },
			{ "rDivider_Highlighter_Hot_Gradient_End", ThemeColor_Divider_Highlighter_Hot_Gradient_End },
			{ "rDivider_Highlighter_Pressed", ThemeColor_Divider_Highlighter_Pressed },
			{ "rButton_Pressed_Gradient_Start", ThemeColor_Button_Pressed_Gradient_Start },
			{ "rButton_Pressed_Gradient_End", ThemeColor_Button_Pressed_Gradient_End },
			{ "rButton_Hot_Gradient_Start", ThemeColor_Button_Hot_Gradient_Start },
			{ "rButton_Hot_Gradient_End", ThemeColor_Button_Hot_Gradient_End },
			{ "rButton_Pressed_Divider", ThemeColor_Button_Pressed_Divider },
			{ "rButton_Light_Disabled_Divider", ThemeColor_Button_Light_Disabled_Divider },
			{ "rButton_Disabled_Gradient_Start", ThemeColor_Button_Disabled_Gradient_Start },
			{ "rButton_Disabled_Gradient_End", ThemeColor_Button_Disabled_Gradient_End },
			{ "rCheckBox_Gradient_Start", ThemeColor_CheckBox_Gradient_Start },
			{ "rCheckBox_Gradient_End", ThemeColor_CheckBox_Gradient_End },
			{ "rScrollBar_Gradient_Start", ThemeColor_ScrollBar_Gradient_Start },
			{ "rScrollBar_Gradient_End", ThemeColor_ScrollBar_Gradient_End },
			{ "rScrollBar_Thumb_Gradient_Start", ThemeColor_ScrollBar_Thumb_Gradient_Start },
			{ "rScrollBar_Thumb_Gradient_End", ThemeColor_ScrollBar_Thumb_Gradient_End },
			{ "rScrollBar_Thumb_Gradient_Hot_Start", ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start },
			{ "rScrollBar_Thumb_Gradient_Hot_End", ThemeColor_ScrollBar_Thumb_Gradient_Hot_End },
			{ "rScrollBar_Thumb_Highlighter", ThemeColor_ScrollBar_Thumb_Highlighter },
			{ "rScrollBar_Thumb_Highlighter_Hot", ThemeColor_ScrollBar_Thumb_Highlighter_Hot },
			{ "rShape", ThemeColor_Shape },
			{ "rShape_Hot", ThemeColor_Shape_Hot },
			{ "rShape_Pressed", ThemeColor_Shape_Pressed },
			{ "rShape_Disabled", ThemeColor_Shape_Disabled },
			{ "rShape_Shadow", ThemeColor_Shape_Shadow },
			{ "rShape_Shadow_Disabled", ThemeColor_Shape_Shadow_Disabled },
			{ "rProgress_Fill_Gradient_Start", ThemeColor_Progress_Fill_Gradient_Start },
			{ "rProgress_Fill_Gradient_End", ThemeColor_Progress_Fill_Gradient_End },
			{ "rProgress_Fill_Highlighter", ThemeColor_Progress_Fill_Highlighter },
			{ "rProgress_Fill_Highlighter_Up", ThemeColor_Progress_Fill_Highlighter_Up },
			{ "rBorder_Window", ThemeColor_Border_Window },
			{ "rStatusBar_Back", ThemeColor_StatusBar_Back },
			{ "rStatusBar_Text", ThemeColor_StatusBar_Text },
			{ "rCaption_Text", ThemeColor_Caption_Text },
			{ "rHeader_Normal_Gradient_Start", ThemeColor_Header_Normal_Gradient_Start },
			{ "rHeader_Normal_Gradient_End", ThemeColor_Header_Normal_Gradient_End },
			{ "rHeader_Hot_Gradient_Start", ThemeColor_Header_Hot_Gradient_Start },
			{ "rHeader_Hot_Gradient_End", ThemeColor_Header_Hot_Gradient_End },
			{ "rListView_Owner_Selected", ThemeColor_ListView_Owner_Selected },
			{ "rSelectedItem_Back", ThemeColor_SelectedItem_Back },
			{ "rSelectedItem_Text", ThemeColor_SelectedItem_Text },
		};

		constexpr COLORREF szConstDark[66]
		{
			RGB(52, 52, 52),											// ThemeColor_Default
			RGB(32, 32, 32),											// ThemeColor_ListView_Color
			RGB(32, 32, 32),											// ThemeColor_TreeView_Color
			RGB(32, 32, 32),											// ThemeColor_Edit_Color
			RGB(37, 37, 37),											// ThemeColor_Edit_Color_Disabled
			RGB(137, 137, 137),											// ThemeColor_Text_1
			RGB(156, 156, 156),											// ThemeColor_Text_2
			RGB(200, 200, 200),											// ThemeColor_Text_3
			RGB(220, 220, 220),											// ThemeColor_Text_4
			RGB(16, 16, 16),											// ThemeColor_MDIWindow
			RGB(78, 78, 78),											// ThemeColor_Default_Gradient_Start
			RGB(61, 61, 61),											// ThemeColor_Default_Gradient_End
			RGB(52, 52, 52),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_Start
			RGB(64, 64, 64),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_End
			RGB(20, 20, 20),											// ThemeColor_Divider_Color
			RGB(38, 38, 38),											// ThemeColor_Divider_Color_Disabled
			RGB(105, 105, 105),											// ThemeColor_Divider_Highlighter
			RGB(125, 125, 125),											// ThemeColor_Divider_Highlighter_Hot
			RGB(38, 38, 38),											// ThemeColor_Divider_Color_Ver2
			RGB(45, 45, 45),											// ThemeColor_Divider_Color_Disabled_Ver2
			RGB(73, 73, 73),											// ThemeColor_Divider_Highlighter_Ver2
			RGB(61, 61, 61),											// ThemeColor_Divider_Highlighter_Disabled_Ver2
			RGB(52, 52, 52),											// ThemeColor_Divider_Highlighter_Gradient_Start
			RGB(72, 72, 72),											// ThemeColor_Divider_Highlighter_Gradient_End
			RGB(52, 52, 52),											// ThemeColor_Divider_Highlighter_Hot_Gradient_Start
			RGB(80, 95, 114),											// ThemeColor_Divider_Highlighter_Hot_Gradient_End
			RGB(74, 102, 150),											// ThemeColor_Divider_Highlighter_Pressed
			RGB(38, 38, 38),											// ThemeColor_Button_Pressed_Gradient_Start
			RGB(29, 29, 29),											// ThemeColor_Button_Pressed_Gradient_End
			RGB(98, 98, 98),											// ThemeColor_Button_Hot_Gradient_Start
			RGB(78, 78, 78),											// ThemeColor_Button_Hot_Gradient_End
			RGB(31, 31, 31),											// ThemeColor_Button_Pressed_Divider
			RGB(79, 79, 79),											// ThemeColor_Button_Light_Disabled_Divider
			RGB(65, 65, 65),											// ThemeColor_Button_Disabled_Gradient_Start
			RGB(57, 57, 57),											// ThemeColor_Button_Disabled_Gradient_End
			RGB(51, 51, 51),											// ThemeColor_CheckBox_Gradient_Start
			RGB(20, 20, 20),											// ThemeColor_CheckBox_Gradient_End
			RGB(39, 39, 39),											// ThemeColor_ScrollBar_Gradient_Start
			RGB(27, 27, 27),											// ThemeColor_ScrollBar_Gradient_End
			RGB(78, 78, 78),											// ThemeColor_ScrollBar_Thumb_Gradient_Start
			RGB(61, 61, 61),											// ThemeColor_ScrollBar_Thumb_Gradient_End
			RGB(98, 98, 98),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start
			RGB(78, 78, 78),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_End
			RGB(105, 105, 105),											// ThemeColor_ScrollBar_Thumb_Highlighter
			RGB(125, 125, 125),											// ThemeColor_ScrollBar_Thumb_Highlighter_Hot
			RGB(190, 190, 190),											// ThemeColor_Shape
			RGB(131, 162, 194),											// ThemeColor_Shape_Hot
			RGB(220, 220, 220),											// ThemeColor_Shape_Pressed
			RGB(110, 110, 110),											// ThemeColor_Shape_Disabled
			RGB(20, 20, 20),											// ThemeColor_Shape_Shadow
			RGB(38, 38, 38),											// ThemeColor_Shape_Shadow_Disabled
			RGB(70, 149, 205),											// ThemeColor_Progress_Fill_Gradient_Start
			RGB(42, 101, 143),											// ThemeColor_Progress_Fill_Gradient_End
			RGB(87, 159, 209),											// ThemeColor_Progress_Fill_Highlighter
			RGB(103, 175, 225),											// ThemeColor_Progress_Fill_Highlighter_Up
			RGB(0, 122, 204),											// ThemeColor_Border_Window
			RGB(32, 32, 32),											// ThemeColor_StatusBar_Back
			RGB(187, 187, 187),											// ThemeColor_StatusBar_Text
			RGB(255, 255, 255),											// ThemeColor_Caption_Text
			RGB(34, 34, 34),											// ThemeColor_Header_Normal_Gradient_Start
			RGB(27, 27, 27),											// ThemeColor_Header_Normal_Gradient_End
			RGB(44, 44, 44),											// ThemeColor_Header_Hot_Gradient_Start
			RGB(37, 37, 37),											// ThemeColor_Header_Hot_Gradient_End
			RGB(160, 201, 235),											// ThemeColor_ListView_Owner_Selected
			RGB(0, 122, 204),											// ThemeColor_SelectedItem_Back
			RGB(187, 187, 187),											// ThemeColor_SelectedItem_Text
		};

		constexpr COLORREF szConstDarkGray[66]
		{
			RGB(83, 83, 83),											// ThemeColor_Default
			RGB(58, 58, 58),											// ThemeColor_ListView_Color
			RGB(58, 58, 58),											// ThemeColor_TreeView_Color
			RGB(58, 58, 58),											// ThemeColor_Edit_Color
			RGB(37, 37, 37),											// ThemeColor_Edit_Color_Disabled
			RGB(137, 137, 137),											// ThemeColor_Text_1
			RGB(156, 156, 156),											// ThemeColor_Text_2
			RGB(200, 200, 200),											// ThemeColor_Text_3
			RGB(220, 220, 220),											// ThemeColor_Text_4
			RGB(48, 48, 48),											// ThemeColor_MDIWindow
			RGB(117, 117, 117),											// ThemeColor_Default_Gradient_Start
			RGB(98, 98, 98),											// ThemeColor_Default_Gradient_End
			RGB(83, 83, 83),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_Start
			RGB(94, 94, 94),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_End
			RGB(40, 40, 40),											// ThemeColor_Divider_Color
			RGB(61, 61, 61),											// ThemeColor_Divider_Color_Disabled
			RGB(145, 145, 145),											// ThemeColor_Divider_Highlighter
			RGB(165, 165, 165),											// ThemeColor_Divider_Highlighter_Hot
			RGB(56, 56, 56),											// ThemeColor_Divider_Color_Ver2
			RGB(96, 96, 96),											// ThemeColor_Divider_Color_Disabled_Ver2
			RGB(124, 124, 124),											// ThemeColor_Divider_Highlighter_Ver2
			RGB(114, 114, 114),											// ThemeColor_Divider_Highlighter_Disabled_Ver2
			RGB(83, 83, 83),											// ThemeColor_Divider_Highlighter_Gradient_Start
			RGB(106, 106, 106),											// ThemeColor_Divider_Highlighter_Gradient_End
			RGB(83, 83, 83),											// ThemeColor_Divider_Highlighter_Hot_Gradient_Start
			RGB(100, 125, 154),											// ThemeColor_Divider_Highlighter_Hot_Gradient_End
			RGB(83, 147, 190),											// ThemeColor_Divider_Highlighter_Pressed
			RGB(63, 63, 63),											// ThemeColor_Button_Pressed_Gradient_Start
			RGB(48, 48, 48),											// ThemeColor_Button_Pressed_Gradient_End
			RGB(127, 127, 127),											// ThemeColor_Button_Hot_Gradient_Start
			RGB(108, 108, 108),											// ThemeColor_Button_Hot_Gradient_End
			RGB(51, 51, 51),											// ThemeColor_Button_Pressed_Divider
			RGB(116, 116, 116),											// ThemeColor_Button_Light_Disabled_Divider
			RGB(100, 100, 100),											// ThemeColor_Button_Disabled_Gradient_Start
			RGB(91, 91, 91),											// ThemeColor_Button_Disabled_Gradient_End
			RGB(71, 71, 71),											// ThemeColor_CheckBox_Gradient_Start
			RGB(40, 40, 40),											// ThemeColor_CheckBox_Gradient_End
			RGB(65, 65, 65),											// ThemeColor_ScrollBar_Gradient_Start
			RGB(54, 54, 54),											// ThemeColor_ScrollBar_Gradient_End
			RGB(139, 139, 139),											// ThemeColor_ScrollBar_Thumb_Gradient_Start
			RGB(107, 107, 107),											// ThemeColor_ScrollBar_Thumb_Gradient_End
			RGB(149, 149, 149),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start
			RGB(117, 117, 117),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_End
			RGB(154, 154, 154),											// ThemeColor_ScrollBar_Thumb_Highlighter
			RGB(164, 164, 164),											// ThemeColor_ScrollBar_Thumb_Highlighter_Hot
			RGB(190, 190, 190),											// ThemeColor_Shape
			RGB(123, 198, 225),											// ThemeColor_Shape_Hot
			RGB(220, 220, 220),											// ThemeColor_Shape_Pressed
			RGB(140, 140, 140),											// ThemeColor_Shape_Disabled
			RGB(40, 40, 40),											// ThemeColor_Shape_Shadow
			RGB(58, 58, 58),											// ThemeColor_Shape_Shadow_Disabled
			RGB(73, 157, 216),											// ThemeColor_Progress_Fill_Gradient_Start
			RGB(50, 120, 169),											// ThemeColor_Progress_Fill_Gradient_End
			RGB(87, 159, 209),											// ThemeColor_Progress_Fill_Highlighter
			RGB(103, 175, 225),											// ThemeColor_Progress_Fill_Highlighter_Up
			RGB(0, 129, 224),											// ThemeColor_Border_Window
			RGB(57, 57, 57),											// ThemeColor_StatusBar_Back
			RGB(200, 200, 200),											// ThemeColor_StatusBar_Text
			RGB(255, 255, 255),											// ThemeColor_Caption_Text
			RGB(60, 60, 60),											// ThemeColor_Header_Normal_Gradient_Start
			RGB(55, 55, 55),											// ThemeColor_Header_Normal_Gradient_End
			RGB(70, 70, 70),											// ThemeColor_Header_Hot_Gradient_Start
			RGB(65, 65, 65),											// ThemeColor_Header_Hot_Gradient_End
			RGB(160, 201, 235),											// ThemeColor_ListView_Owner_Selected
			RGB(0, 129, 224),											// ThemeColor_SelectedItem_Back
			RGB(200, 200, 200),											// ThemeColor_SelectedItem_Text
		};

		constexpr COLORREF szConstGray[66]
		{
			RGB(184, 184, 184),											// ThemeColor_Default
			RGB(238, 238, 238),											// ThemeColor_ListView_Color
			RGB(238, 238, 238),											// ThemeColor_TreeView_Color
			RGB(238, 238, 238),											// ThemeColor_Edit_Color
			RGB(238, 238, 238),											// ThemeColor_Edit_Color_Disabled
			RGB(141, 141, 141),											// ThemeColor_Text_1
			RGB(96, 96, 96),											// ThemeColor_Text_2
			RGB(32, 32, 32),											// ThemeColor_Text_3
			RGB(16, 16, 16),											// ThemeColor_Text_4
			RGB(123, 123, 123),											// ThemeColor_MDIWindow
			RGB(229, 229, 229),											// ThemeColor_Default_Gradient_Start
			RGB(190, 190, 190),											// ThemeColor_Default_Gradient_End
			RGB(184, 184, 184),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_Start
			RGB(192, 192, 192),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_End
			RGB(81, 81, 81),											// ThemeColor_Divider_Color
			RGB(140, 140, 140),											// ThemeColor_Divider_Color_Disabled
			RGB(246, 246, 246),											// ThemeColor_Divider_Highlighter
			RGB(246, 246, 246),											// ThemeColor_Divider_Highlighter_Hot
			RGB(153, 153, 153),											// ThemeColor_Divider_Color_Ver2
			RGB(196, 196, 196),											// ThemeColor_Divider_Color_Disabled_Ver2
			RGB(212, 212, 212),											// ThemeColor_Divider_Highlighter_Ver2
			RGB(164, 164, 164),											// ThemeColor_Divider_Highlighter_Disabled_Ver2
			RGB(184, 184, 184),											// ThemeColor_Divider_Highlighter_Gradient_Start
			RGB(199, 199, 199),											// ThemeColor_Divider_Highlighter_Gradient_End
			RGB(184, 184, 184),											// ThemeColor_Divider_Highlighter_Hot_Gradient_Start
			RGB(100, 125, 154),											// ThemeColor_Divider_Highlighter_Hot_Gradient_End
			RGB(83, 147, 190),											// ThemeColor_Divider_Highlighter_Pressed
			RGB(139, 139, 139),											// ThemeColor_Button_Pressed_Gradient_Start
			RGB(130, 130, 130),											// ThemeColor_Button_Pressed_Gradient_End
			RGB(239, 239, 239),											// ThemeColor_Button_Hot_Gradient_Start
			RGB(200, 200, 200),											// ThemeColor_Button_Hot_Gradient_End
			RGB(121, 121, 121),											// ThemeColor_Button_Pressed_Divider
			RGB(223, 223, 223),											// ThemeColor_Button_Light_Disabled_Divider
			RGB(207, 207, 207),											// ThemeColor_Button_Disabled_Gradient_Start
			RGB(187, 187, 187),											// ThemeColor_Button_Disabled_Gradient_End
			RGB(111, 111, 111),											// ThemeColor_CheckBox_Gradient_Start
			RGB(81, 81, 81),											// ThemeColor_CheckBox_Gradient_End
			RGB(221, 221, 221),											// ThemeColor_ScrollBar_Gradient_Start
			RGB(189, 189, 189),											// ThemeColor_ScrollBar_Gradient_End
			RGB(161, 161, 161),											// ThemeColor_ScrollBar_Thumb_Gradient_Start
			RGB(129, 129, 129),											// ThemeColor_ScrollBar_Thumb_Gradient_End
			RGB(171, 171, 171),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start
			RGB(139, 139, 139),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_End
			RGB(174, 174, 174),											// ThemeColor_ScrollBar_Thumb_Highlighter
			RGB(184, 184, 184),											// ThemeColor_ScrollBar_Thumb_Highlighter_Hot
			RGB(76, 76, 76),											// ThemeColor_Shape
			RGB(123, 198, 225),											// ThemeColor_Shape_Hot
			RGB(55, 55, 55),											// ThemeColor_Shape_Pressed
			RGB(129, 129, 129),											// ThemeColor_Shape_Disabled
			RGB(220, 220, 220),											// ThemeColor_Shape_Shadow
			RGB(190, 190, 190),											// ThemeColor_Shape_Shadow_Disabled
			RGB(163, 57, 168),											// ThemeColor_Progress_Fill_Gradient_Start
			RGB(133, 48, 137),											// ThemeColor_Progress_Fill_Gradient_End
			RGB(186, 71, 192),											// ThemeColor_Progress_Fill_Highlighter
			RGB(173, 62, 179),											// ThemeColor_Progress_Fill_Highlighter_Up
			RGB(0, 144, 255),											// ThemeColor_Border_Window
			RGB(128, 128, 128),											// ThemeColor_StatusBar_Back
			RGB(24, 24, 24),											// ThemeColor_StatusBar_Text
			RGB(20, 20, 20),											// ThemeColor_Caption_Text
			RGB(166, 166, 166),											// ThemeColor_Header_Normal_Gradient_Start
			RGB(147, 147, 147),											// ThemeColor_Header_Normal_Gradient_End
			RGB(176, 176, 176),											// ThemeColor_Header_Hot_Gradient_Start
			RGB(157, 157, 157),											// ThemeColor_Header_Hot_Gradient_End
			RGB(242, 176, 173),											// ThemeColor_ListView_Owner_Selected											
			RGB(117, 197, 255),											// ThemeColor_SelectedItem_Back
			RGB(32, 32, 32),											// ThemeColor_SelectedItem_Text
		};

		constexpr COLORREF szConstLight[66]
		{
			RGB(214, 214, 214),											// ThemeColor_Default
			RGB(255, 255, 255),											// ThemeColor_ListView_Color
			RGB(255, 255, 255),											// ThemeColor_TreeView_Color
			RGB(255, 255, 255),											// ThemeColor_Edit_Color
			RGB(235, 235, 235),											// ThemeColor_Edit_Color_Disabled
			RGB(158, 158, 158),											// ThemeColor_Text_1
			RGB(96, 96, 96),											// ThemeColor_Text_2
			RGB(32, 32, 32),											// ThemeColor_Text_3
			RGB(16, 16, 16),											// ThemeColor_Text_4
			RGB(153, 153, 153),											// ThemeColor_MDIWindow
			RGB(253, 253, 253),											// ThemeColor_Default_Gradient_Start
			RGB(233, 233, 233),											// ThemeColor_Default_Gradient_End
			RGB(214, 214, 214),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_Start
			RGB(223, 223, 223),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_End
			RGB(100, 100, 100),											// ThemeColor_Divider_Color
			RGB(172, 172, 172),											// ThemeColor_Divider_Color_Disabled
			RGB(255, 255, 255),											// ThemeColor_Divider_Highlighter
			RGB(255, 255, 255),											// ThemeColor_Divider_Highlighter_Hot
			RGB(153, 153, 153),											// ThemeColor_Divider_Color_Ver2
			RGB(196, 196, 196),											// ThemeColor_Divider_Color_Disabled_Ver2
			RGB(232, 232, 232),											// ThemeColor_Divider_Highlighter_Ver2
			RGB(200, 200, 200),											// ThemeColor_Divider_Highlighter_Disabled_Ver2
			RGB(214, 214, 214),											// ThemeColor_Divider_Highlighter_Gradient_Start
			RGB(232, 232, 232),											// ThemeColor_Divider_Highlighter_Gradient_End
			RGB(214, 214, 214),											// ThemeColor_Divider_Highlighter_Hot_Gradient_Start
			RGB(100, 125, 154),											// ThemeColor_Divider_Highlighter_Hot_Gradient_End
			RGB(83, 147, 190),											// ThemeColor_Divider_Highlighter_Pressed
			RGB(169, 169, 169),											// ThemeColor_Button_Pressed_Gradient_Start
			RGB(160, 160, 160),											// ThemeColor_Button_Pressed_Gradient_End
			RGB(243, 243, 243),											// ThemeColor_Button_Hot_Gradient_Start
			RGB(223, 223, 223),											// ThemeColor_Button_Hot_Gradient_End
			RGB(121, 121, 121),											// ThemeColor_Button_Pressed_Divider
			RGB(240, 240, 240),											// ThemeColor_Button_Light_Disabled_Divider
			RGB(233, 233, 233),											// ThemeColor_Button_Disabled_Gradient_Start
			RGB(224, 224, 224),											// ThemeColor_Button_Disabled_Gradient_End
			RGB(131, 131, 131),											// ThemeColor_CheckBox_Gradient_Start
			RGB(100, 100, 100),											// ThemeColor_CheckBox_Gradient_End
			RGB(231, 231, 231),											// ThemeColor_ScrollBar_Gradient_Start
			RGB(201, 201, 201),											// ThemeColor_ScrollBar_Gradient_End
			RGB(179, 179, 179),											// ThemeColor_ScrollBar_Thumb_Gradient_Start
			RGB(142, 142, 142),											// ThemeColor_ScrollBar_Thumb_Gradient_End
			RGB(189, 189, 189),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start
			RGB(152, 152, 152),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_End
			RGB(190, 190, 190),											// ThemeColor_ScrollBar_Thumb_Highlighter
			RGB(200, 200, 200),											// ThemeColor_ScrollBar_Thumb_Highlighter_Hot
			RGB(94, 94, 94),											// ThemeColor_Shape
			RGB(123, 198, 225),											// ThemeColor_Shape_Hot
			RGB(71, 71, 71),											// ThemeColor_Shape_Pressed
			RGB(149, 149, 149),											// ThemeColor_Shape_Disabled
			RGB(240, 240, 240),											// ThemeColor_Shape_Shadow
			RGB(210, 210, 210),											// ThemeColor_Shape_Shadow_Disabled
			RGB(163, 57, 168),											// ThemeColor_Progress_Fill_Gradient_Start
			RGB(133, 48, 137),											// ThemeColor_Progress_Fill_Gradient_End
			RGB(186, 71, 192),											// ThemeColor_Progress_Fill_Highlighter
			RGB(173, 62, 179),											// ThemeColor_Progress_Fill_Highlighter_Up
			RGB(0, 144, 255),											// ThemeColor_Border_Window
			RGB(148, 148, 148),											// ThemeColor_StatusBar_Back
			RGB(34, 34, 34),											// ThemeColor_StatusBar_Text
			RGB(20, 20, 20),											// ThemeColor_Caption_Text
			RGB(196, 196, 196),											// ThemeColor_Header_Normal_Gradient_Start
			RGB(177, 177, 177),											// ThemeColor_Header_Normal_Gradient_End
			RGB(206, 206, 206),											// ThemeColor_Header_Hot_Gradient_Start
			RGB(187, 187, 187),											// ThemeColor_Header_Hot_Gradient_End
			RGB(242, 176, 173),											// ThemeColor_ListView_Owner_Selected
			RGB(117, 197, 255),											// ThemeColor_SelectedItem_Back
			RGB(32, 32, 32),											// ThemeColor_SelectedItem_Text
		};

		constexpr COLORREF szConstNightBlue[66]
		{
			RGB(34, 37, 50),											// ThemeColor_Default
			RGB(18, 19, 22),											// ThemeColor_ListView_Color
			RGB(18, 19, 22),											// ThemeColor_TreeView_Color
			RGB(18, 19, 22),											// ThemeColor_Edit_Color
			RGB(25, 27, 35),											// ThemeColor_Edit_Color_Disabled
			RGB(92, 100, 126),											// ThemeColor_Text_1
			RGB(118, 126, 154),											// ThemeColor_Text_2
			RGB(163, 168, 182),											// ThemeColor_Text_3
			RGB(180, 184, 195),											// ThemeColor_Text_4
			RGB(18, 19, 22),											// ThemeColor_MDIWindow
			RGB(36, 39, 53),											// ThemeColor_Default_Gradient_Start
			RGB(56, 62, 84),											// ThemeColor_Default_Gradient_End
			RGB(49, 53, 68),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_Start
			RGB(49, 53, 68),											// ThemeColor_Divider_Highlighter_Disabled_Gradient_End
			RGB(31, 31, 33),											// ThemeColor_Divider_Color
			RGB(39, 39, 41),											// ThemeColor_Divider_Color_Disabled
			RGB(62, 67, 85),											// ThemeColor_Divider_Highlighter
			RGB(90, 99, 126),											// ThemeColor_Divider_Highlighter_Hot
			RGB(49, 54, 70),											// ThemeColor_Divider_Color_Ver2
			RGB(41, 45, 58),											// ThemeColor_Divider_Color_Disabled_Ver2
			RGB(73, 80, 101),											// ThemeColor_Divider_Highlighter_Ver2
			RGB(62, 67, 83),											// ThemeColor_Divider_Highlighter_Disabled_Ver2
			RGB(50, 55, 73),											// ThemeColor_Divider_Highlighter_Gradient_Start
			RGB(62, 67, 89),											// ThemeColor_Divider_Highlighter_Gradient_End
			RGB(58, 63, 82),											// ThemeColor_Divider_Highlighter_Hot_Gradient_Start
			RGB(69, 78, 99),											// ThemeColor_Divider_Highlighter_Hot_Gradient_End
			RGB(0, 144, 255),											// ThemeColor_Divider_Highlighter_Pressed	
			RGB(38, 41, 53),											// ThemeColor_Button_Pressed_Gradient_Start
			RGB(28, 31, 38),											// ThemeColor_Button_Pressed_Gradient_End
			RGB(98, 108, 138),											// ThemeColor_Button_Hot_Gradient_Start
			RGB(78, 86, 109),											// ThemeColor_Button_Hot_Gradient_End
			RGB(31, 33, 41),											// ThemeColor_Button_Pressed_Divider
			RGB(78, 86, 109),											// ThemeColor_Button_Light_Disabled_Divider
			RGB(65, 70, 90),											// ThemeColor_Button_Disabled_Gradient_Start
			RGB(57, 63, 81),											// ThemeColor_Button_Disabled_Gradient_End
			RGB(50, 55, 71),											// ThemeColor_CheckBox_Gradient_Start
			RGB(28, 31, 38),											// ThemeColor_CheckBox_Gradient_End
			RGB(63, 69, 88),											// ThemeColor_ScrollBar_Gradient_Start
			RGB(53, 58, 74),											// ThemeColor_ScrollBar_Gradient_End
			RGB(78, 86, 109),											// ThemeColor_ScrollBar_Thumb_Gradient_Start
			RGB(61, 67, 86),											// ThemeColor_ScrollBar_Thumb_Gradient_End
			RGB(98, 108, 138),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start
			RGB(78, 86, 109),											// ThemeColor_ScrollBar_Thumb_Gradient_Hot_End
			RGB(105, 116, 148),											// ThemeColor_ScrollBar_Thumb_Highlighter
			RGB(125, 135, 162),											// ThemeColor_ScrollBar_Thumb_Highlighter_Hot
			RGB(190, 190, 190),											// ThemeColor_Shape
			RGB(0, 144, 255),											// ThemeColor_Shape_Hot
			RGB(220, 220, 220),											// ThemeColor_Shape_Pressed
			RGB(110, 110, 110),											// ThemeColor_Shape_Disabled
			RGB(20, 20, 20),											// ThemeColor_Shape_Shadow
			RGB(38, 38, 38),											// ThemeColor_Shape_Shadow_Disabled
			RGB(70, 149, 205),											// ThemeColor_Progress_Fill_Gradient_Start
			RGB(42, 101, 143),											// ThemeColor_Progress_Fill_Gradient_End
			RGB(87, 159, 209),											// ThemeColor_Progress_Fill_Highlighter
			RGB(103, 175, 225),											// ThemeColor_Progress_Fill_Highlighter_Up
			RGB(68, 173, 255),											// ThemeColor_Border_Window
			RGB(18, 19, 22),											// ThemeColor_StatusBar_Back
			RGB(139, 142, 151),											// ThemeColor_StatusBar_Text
			RGB(255, 255, 255),											// ThemeColor_Caption_Text
			RGB(48, 52, 69),											// ThemeColor_Header_Normal_Gradient_Start
			RGB(48, 52, 69),											// ThemeColor_Header_Normal_Gradient_End
			RGB(58, 64, 84),											// ThemeColor_Header_Hot_Gradient_Start
			RGB(58, 64, 84),											// ThemeColor_Header_Hot_Gradient_End
			RGB(54, 108, 152),											// ThemeColor_ListView_Owner_Selected
			RGB(0, 129, 224),											// ThemeColor_SelectedItem_Back
			RGB(200, 200, 200),											// ThemeColor_SelectedItem_Text
			//58, 64, 84
		};

		COLORREF* GetThemeColors(const Theme theme)
		{
			switch (theme)
			{
			case Theme_Light:
				return const_cast<COLORREF*>(szConstLight);
			case Theme_Gray:
				return const_cast<COLORREF*>(szConstGray);
			case Theme_DarkGray:
				return const_cast<COLORREF*>(szConstDarkGray);
			case Theme_Dark:
				return const_cast<COLORREF*>(szConstDark);
			case Theme_NightBlue:
				return const_cast<COLORREF*>(szConstNightBlue);
			}

			return nullptr;
		}

		COLORREF GetThemeSysColor(const Theme theme, const ThemeColor color)
		{
			if (theme == Theme_DarkGray)
				return szConstDarkGray[color - 1];
			else if (theme == Theme_Dark)
				return szConstDark[color - 1];
			else if (theme == Theme_Light)
				return szConstLight[color - 1];
			else if (theme == Theme_Gray)
				return szConstGray[color - 1];
			else if (theme == Theme_NightBlue)
				return szConstNightBlue[color - 1];
			else
				return 0;
		}
	}
}