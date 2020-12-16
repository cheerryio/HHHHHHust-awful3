import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles((theme: Theme) =>
    createStyles({
        hightLight: {
            backgroundColor: 'gray',
        },
        numberIconNormal: {
            margin: 0,
            padding: 0,
            height: '8vmin',
            width: '8vmin',
            display: 'flex',
            borderRadius: '20%',
            '&:hover': {
                backgroundColor: 'grey',
            },
        },
        numberContainerNormal: {
            margin: 0,
            padding: 0,
            [theme.breakpoints.up('sm')]: {
                width: '18vmin',
                height: '50vmin',
                display: 'flex',
                transform: 'translate(0%,-50%)',
                position: 'fixed',
                top: '50vh',
                right: '3vw',
            },
            [theme.breakpoints.only('sm')]: {
                width: '8vmin',
                height: '80vh',
                display: 'flex',
                transform: 'translate(0%,-50%)',
                position: 'fixed',
                top: '50vh',
                right: '1vw',
            },
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
    }),
);

export default useStyles;
