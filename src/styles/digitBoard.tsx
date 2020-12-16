import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles(({ palette, shadows, spacing, breakpoints }: Theme) =>
    createStyles({
        modalContainer: {
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
        },
        modal: {
            display: 'flex',
            flexDirection: 'column',

            borderRadius: 10,
            transform: 'translate(-50%,-50%)',
            position: 'fixed',
            top: '50%',
            left: '50%',

            //boxShadow: shadows[5],
            //padding: spacing(1),
        },
        digitsContainerLine: {
            display: 'flex',
            width: '40vmin',
            //maxWidth:'35%',
            //margin:'auto',
            backgroundColor: palette.primary.main,
        },
        digitButtonContainer: {
            width: '12vmin',
            padding: '0.3vmin',
            margin: '0.3vmin',
        },
        digitButton: {
            //color:palette.secondary.light,
            fill: palette.secondary.contrastText,
            borderRadius: 15,
            width: '12vmin',
            height: '12vmin',
            //margin:spacing(2),
            padding: 0,
        },
        backdrop: {
            background: palette.primary.main,
        },
    }),
);

export default useStyles;
