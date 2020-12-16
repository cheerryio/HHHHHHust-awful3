import { Theme } from '@material-ui/core/styles/createMuiTheme';
import { createStyles, makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles(({ palette, shadows, spacing, breakpoints }: Theme) =>
    createStyles({
        success: { backgroundColor: 'red' },
        error: { backgroundColor: 'purple' },
        warning: { backgroundColor: 'red' },
        info: { backgroundColor: 'purple' },
    }),
);

export default useStyles;
