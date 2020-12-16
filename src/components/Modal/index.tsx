import React, { FC, memo } from 'react';

import Modal from '@material-ui/core/Modal';
import Typography from '@material-ui/core/Typography';
import Slide, { SlideProps } from '@material-ui/core/Slide';

import useStyles from '../../styles/modal';

export interface Props {
    open: boolean;
    title: string;
    direction?: SlideProps['direction'];
    onClose: () => void;
}

const customModal: FC<Props> = memo(({ open, title, direction, children, onClose }) => {
    const classes = useStyles();
    const leaveDirection = direction === 'left' ? 'right' : 'left';
    return (
        <Modal
            open={open}
            onClose={onClose}
            aria-labelledby='simple-modal-title'
            aria-describedby='simple-modal-description'
            disableEnforceFocus>
            <Slide direction={open ? direction || 'right' : leaveDirection} in={open} mountOnEnter unmountOnExit>
                <div className={classes.modal}>
                    <div className={classes.modalHeader}>
                        <Typography variant='h5' className={classes.modalTitle}>
                            {title}
                        </Typography>
                    </div>
                    {children}
                </div>
            </Slide>
        </Modal>
    );
});

export default customModal;
