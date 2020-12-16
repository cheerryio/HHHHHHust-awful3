import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles(({ palette, shadows, spacing, breakpoints }: Theme) =>
    createStyles({
        playBoardContainer: {
            display: 'flex',
            position: 'fixed',
            left: '50%',
            width: '84vmin',
            height: '83.5vmin',

            top: '50%',
            transform: 'translate(-50%,-50%)',
            [breakpoints.down('sm')]: {
                transform: 'translate(-50%,0%)',
                top: '10%',
            },
            backgroundColor: `${palette.boardBackgroundColor?.main}`,
        },
        PlayBoardLine: {
            margin: spacing(0),
            padding: spacing(0),
            height: '9vmin',
        },
        splitBorder: {
            padding: '0.3vmin',
            margin: 0,
            //borderTop:`dashed 0.3vmin ${palette.numberBackgroundColor?.contrastText}`,
            borderBottom: `dashed 0.3vmin ${palette.numberBackgroundColor?.contrastText}`,
            borderRight: `dashed 0.3vmin ${palette.numberBackgroundColor?.contrastText}`,
            width: '9.3vmin',
            height: '9vmin',
        },
        playBoardBlockContainer: {
            position: 'relative',
            margin: spacing(0),
            padding: spacing(0),
            cursor: 'pointer',
            textAlign: 'center',
            verticalAlign: '',
            borderRadius: '0',
            backgroundColor: `${palette.numberBackgroundColor?.main}`,
            fill: `${palette.numberBackgroundColor?.contrastText}`,
        },
        hightLight: {
            margin: 0,
            padding: 0,

            backgroundColor: `${palette.hightLightNumberBackgroundColor?.main}`,
            color: `${palette.hightLightNumberBackgroundColor?.contrastText}`,
        },
        numberIconNormal: {
            margin: 0,
            padding: 0,
            width: '8.25vmin',
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
            paddingRight: '0.3vmin',
            borderRight: '0.3vmin solid black',
        },
        leftPadding: {
            paddingLeft: '0.3vmin',
        },
        bottomPaddingBorder: {
            paddingBottom: '0.3vmin',
            borderBottom: '0.3vmin solid black',
        },
        topPadding: {
            paddingTop: '0.3vmin',
        },
        unchangeableBlock: {
            backgroundColor: 'orange',
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
            paddingTop: '0.4vmin',
            //background:"blue",
        },
        conflictOne: {
            backgroundColor: 'blue',
        },
        conflictTwo: {
            backgroundColor: 'red',
        },
        conflictThree: {
            backgroundColor: 'black',
        },
        conflictFour: {
            backgroundColor: 'yellow',
        },
        conflictFive: {
            backgroundColor: 'green',
        },
        conflictSix: {
            backgroundColor: 'purple',
        },
        conflictSeven: {
            backgroundColor: 'gray',
        },
        conflictEight: {
            backgroundColor: '#eeeeee',
        },
        conflictNine: {
            backgroundColor: 'red',
        },
    }),
);

export default useStyles;
