import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles(({ palette, shadows, spacing, breakpoints }: Theme) => {
    let screenNormal: boolean = window.innerWidth > window.innerHeight ? true : false; //true 是横屏，false是竖屏
    return createStyles({
        playBoardContainer: {
            display: 'flex',
            position: 'fixed',
            left: '50%',
            width: '84.6vmin',
            ...(() => {
                if (screenNormal)
                    return {
                        top: '50%',
                        transform: 'translate(-50%,-50%)',
                    };
                return {
                    transform: 'translate(-50%,0%)',
                    top: '10%',
                };
            })(),
            backgroundColor: `${palette.boardBackgroundColor?.main}`,
        },
        PlayBoardLine: {
            margin: spacing(0),
            padding: spacing(0),
        },
        splitBorder: {
            padding: '0.5vmin',
            margin: 0,
            //borderTop:`dashed 0.3vmin ${palette.numberBackgroundColor?.contrastText}`,
            borderBottom: `dashed 0.3vmin ${palette.numberBackgroundColor?.contrastText}`,
            borderRight: `dashed 0.3vmin ${palette.numberBackgroundColor?.contrastText}`,
            width: '9.4vmin',
            height: '9.4vmin',
        },
        playBoardBlockContainer: {
            position: 'relative',
            margin: spacing(0),
            padding: spacing(0),
            cursor: 'pointer',
            textAlign: 'center',
            verticalAlign: '',
            backgroundColor: `${palette.numberBackgroundColor?.main}`,
            fill: `${palette.numberBackgroundColor?.contrastText}`,
        },
        hightLight: {
            backgroundColor: `${palette.hightLightNumberBackgroundColor?.main}`,
            color: `${palette.hightLightNumberBackgroundColor?.contrastText}`,
        },
        numberIconNormal: {
            margin: 0,
            padding: 0,
            width: '8vmin',
            //height:'8.7vmin',
            borderRadius: '20%',
        },
        hidenullIcon: {
            display: 'none',
        },
        backdrop: {
            background: palette.primary.light,
        },

        rightPaddingBorder: {
            //paddingRight: '0.3vmin',
            borderRight: `0.3vmin solid ${palette.numberBackgroundColor?.contrastText}`,
        },

        bottomPaddingBorder: {
            //paddingBottom: '0.3vmin',
            borderBottom: `0.3vmin solid ${palette.numberBackgroundColor?.contrastText}`,
        },

        unchangeableBlock: {
            backgroundColor: `${palette.constNumberBackgroundColor?.main}`,
            color: `${palette.constNumberBackgroundColor?.contrastText}`,
        },
        optionNumberIcon: {
            //background:"blue",
            width: '2.5vmin',
            height: '2.5vmin',
            cursor: 'pointer',

            backgroundColor: `${palette.optionNumberBackgroudColor?.main}`,
            fill: `${palette.optionNumberBackgroudColor?.contrastText}`,
        },
        optionNumberBlock: {
            paddingTop: '0.3vmin',
            //margin:"0vmin",
            position: 'relative',
            fontSize: '0rem',
        },
        optionalNumberTopPadding: {
            //paddingTop: '0.4vmin',
            //background:"blue",
        },
        conflictNumber: {
            color: `${palette.conflictNumberBackgroundColor?.contrastText}`,
            background: `${palette.conflictNumberBackgroundColor?.main}`,
        },
    });
});

export default useStyles;
