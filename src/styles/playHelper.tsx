import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles((theme: Theme) => {
    let screenNormal: boolean = window.innerWidth > window.innerHeight ? true : false; //true 是横屏，false是竖屏
    return createStyles({
        hightLight: {
            backgroundColor: `${theme.palette.hightLightNumberBackgroundColor?.main}`,
            color: `${theme.palette.hightLightNumberBackgroundColor?.contrastText}`,
        },
        numberIconNormaContainer: {
            margin: 0,
            padding: 0,
            height: '16vmin',
            width: '16vmin',
            display: 'flex',
        },
        numberIconNormal: {
            margin: 0,
            padding: 0,
            height: '16vmin',
            width: '16vmin',
            display: 'flex',
            borderRadius: '20%',
            color: `${theme.palette.boardBackgroundColor?.contrastText}`,
            backgroundColor: `${theme.palette.boardBackgroundColor?.main}`,
            '&:hover': {
                backgroundColor: `${theme.palette.hightLightNumberBackgroundColor?.main}`,
                color: `${theme.palette.hightLightNumberBackgroundColor?.contrastText}`,
            },
        },
        numberContainerNormal: {
            margin: 0,
            padding: 0,
            //height: '50vmin',
            display: 'flex',
            position: 'fixed',
            ...(() => {
                if (screenNormal)
                    return { width: '32vmin', transform: 'translate(0%,-50%)', top: '50vh', right: '3vw' };
                return { width: '60vmin', transform: 'translate(-50%,0%)', left: '50vw', down: '3vh' };
            })(),
        },
        haveBorder: {
            border: '0.7vmin solid black',
            borderRadius: '20%',
        },
        iconButtonContainer: {
            height: '10vmin',
            width: '10vmin',
            '&:hover': {
                backgroundColor: 'grey',
            },
        },
        iconButtonIcon: {
            height: '5.5vmin',
            width: '5.5vmin',
            //fontSize:"6vmin",
            //color:"#000000",
        },
        hideElement: {
            display: 'none',
        },
        buttonContainers: {
            position: 'fixed',
            display: 'flex',
            transform: 'translate(0%,-50%)',
            top: '50vh',
            left: '2vw',
            [theme.breakpoints.up('sm')]: {
                width: '26vmin',
                height: '8vmin',
            },
            [theme.breakpoints.only('sm')]: {
                width: '8vmin',
                height: '26vmin',
            },
        },
    });
});

export default useStyles;
