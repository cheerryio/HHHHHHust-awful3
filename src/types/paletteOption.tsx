import { Palette, PaletteOptions, PaletteColorOptions, PaletteColor } from '@material-ui/core/styles/createPalette';
declare module '@material-ui/core/styles/createPalette' {
    interface PaletteOptions {
        //棋盘的背景颜色
        boardBackgroundColor?: PaletteColorOptions;

        //填入的数字的颜色
        //numberColor?:PaletteColorOptions,

        //填入的数字的背景颜色
        numberBackgroundColor?: PaletteColorOptions;

        //冲突的数字的颜色
        //conflictNumberColor?:PaletteColorOptions,

        //冲突的数字的背景颜色
        conflictNumberBackgroundColor?: PaletteColorOptions;

        //不可变的数字的颜色
        //constNumberColor?:PaletteColorOptions,

        //不可变的数字的背景颜色
        constNumberBackgroundColor?: PaletteColorOptions;

        //可选数字的颜色
        //optionNumberColor?:PaletteColorOptions,

        //可选数字的背景颜色
        optionNumberBackgroudColor?: PaletteColorOptions;

        //按钮的颜色
        //buttonColor?:PaletteColorOptions,

        //按钮的背景颜色
        buttonBackgroundColor?: PaletteColorOptions;

        //高亮数字的颜色
        //hightLightNumberColor?:PaletteColorOptions,

        //高亮数字的背景颜色
        hightLightNumberBackgroundColor?: PaletteColorOptions;
    }
    interface Palette {
        //棋盘的背景颜色
        boardBackgroundColor?: PaletteColor;

        //填入的数字的颜色
        //numberColor?:PaletteColor,

        //填入的数字的背景颜色
        numberBackgroundColor?: PaletteColor;

        //冲突的数字的颜色
        //conflictNumberColor?:PaletteColor,

        //冲突的数字的背景颜色
        conflictNumberBackgroundColor?: PaletteColor;

        //不可变的数字的颜色
        //constNumberColor?:PaletteColor,

        //不可变的数字的背景颜色
        constNumberBackgroundColor?: PaletteColor;

        //可选数字的颜色
        //optionNumberColor?:PaletteColor,

        //可选数字的背景颜色
        optionNumberBackgroudColor?: PaletteColor;

        //按钮的颜色
        //buttonColor?:PaletteColor,

        //按钮的背景颜色
        buttonBackgroundColor?: PaletteColor;

        //高亮数字的颜色
        //hightLightNumberColor?:PaletteColorOptions,

        //高亮数字的背景颜色
        hightLightNumberBackgroundColor?: PaletteColor;
    }
}
