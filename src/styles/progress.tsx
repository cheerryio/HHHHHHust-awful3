import purple from '@material-ui/core/colors/purple';
import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/styles';

const useStyles = makeStyles(({ zIndex }: Theme) =>
    createStyles({
        progress: {
            position: 'fixed',
            zIndex: zIndex.modal + 1,
            top: 0,
            left: 0,
            width: '100%',
        },
        color: {
            backgroundColor: purple[500],
        },
        barColor: {
            backgroundColor: purple[100],
        },
    }),
);

export default useStyles;
