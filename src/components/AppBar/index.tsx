import React, { FC, memo, useState, useMemo, MouseEventHandler, useContext } from 'react';
import { AppBar, Menu, MenuItem, IconButton, Toolbar, Typography, Slide, Collapse } from '@material-ui/core';
import { useScrollTrigger } from '@material-ui/core';

import { Brightness4, KeyboardArrowDown, KeyboardArrowUp } from '@material-ui/icons';
import HelpIcon from '@material-ui/icons/HelpOutline';
import MenuIcon from '@material-ui/icons/Menu';
import PersonIcon from '@material-ui/icons/Person';
import ChatIcon from '@material-ui/icons/Chat';
import Drawer from '../Drawer';

import Modal from '../Modal';
import classNames from 'classnames';
import useStyles from '../../styles/appBar';

import { Props } from '../../containers/AppBar';
import { ThemeContext } from '../../styles/withRoot';
import TipButton from '../SmallComponents/TipButton';

import Messenger from '../../containers/Messenger';

const Bar: FC<Props> = memo(({ updateSudokuAction, playRoundBackwardAction }) => {
    const classes = useStyles();

    const [open, setOpen] = useState(true);
    const [right, setRight] = useState(false);
    const [anchorEl, setAnchorEl] = useState<Element | null>(null);
    const [helper, setHelper] = useState(false);
    const [drawer, setDrawer] = useState(false);
    const [messenger, setMessenger] = useState(false);
    const { darkMode, setDarkMode } = useContext(ThemeContext); // eslint-disable-line

    const handleClickPerson: MouseEventHandler = ({ currentTarget }) => {
        setAnchorEl(currentTarget);
    };
    const handleClosePerson = () => {
        setAnchorEl(null);
    };
    const toggleOpen = () => {
        setOpen((open) => !open);
    };
    const toggleRight = () => {
        setRight((right) => !right);
    };
    const toggleHelper = () => {
        setHelper((prev) => !prev);
    };
    const toggleDrawer = () => {
        setDrawer((drawer) => !drawer);
    };
    const toggleMessenger = () => {
        setMessenger((messenger) => !messenger);
    };

    const HelpMessage = useMemo(() => <Typography>这里是我们要做的东西</Typography>, []);

    const trigger = useScrollTrigger({ target: undefined });

    return (
        <>
            <Slide appear={false} direction='down' in={open}>
                <AppBar
                    position='fixed'
                    className={classNames(classes.appBar, {
                        [classes.appBarShift]: right,
                    })}>
                    <Toolbar
                        disableGutters={!right}
                        classes={{ gutters: classes.appBarGutters, regular: classes.regular }}>
                        {useMemo(
                            () => (
                                <IconButton
                                    color='inherit'
                                    className={classNames(classes.menuButton, {})}
                                    onClick={() => {
                                        toggleDrawer();
                                        toggleRight();
                                    }}>
                                    <MenuIcon />
                                </IconButton>
                            ),
                            // eslint-disable-next-line
                            [right, classes.menuButton],
                        )}

                        {useMemo(
                            () => (
                                <div className={classNames(classes.rightButtons)}>
                                    <TipButton
                                        TooltipProp={{ title: 'Messenger', children: <></> }}
                                        IconButtonProp={{ onClick: toggleMessenger, color: 'inherit' }}>
                                        <ChatIcon />
                                    </TipButton>
                                    <TipButton
                                        TooltipProp={{ title: 'Help', children: <></> }}
                                        IconButtonProp={{ onClick: toggleHelper, color: 'inherit' }}>
                                        <HelpIcon />
                                    </TipButton>
                                    <TipButton
                                        TooltipProp={{ title: 'Dark', children: <></> }}
                                        IconButtonProp={{
                                            onClick: setDarkMode,
                                            color: 'inherit',
                                        }}>
                                        <Brightness4 />
                                    </TipButton>
                                    <TipButton
                                        TooltipProp={{ title: 'Logout', children: <></> }}
                                        IconButtonProp={{ onClick: handleClickPerson, color: 'inherit' }}>
                                        <PersonIcon />
                                    </TipButton>
                                </div>
                            ),
                            // eslint-disable-next-line
                            [right, classes.rightButtons],
                        )}
                        <Menu anchorEl={anchorEl} open={Boolean(anchorEl)} onClose={handleClosePerson}>
                            <MenuItem>退出</MenuItem>
                        </Menu>
                    </Toolbar>
                </AppBar>
            </Slide>
            <Collapse in={messenger} classes={{ container: classes.collapse }}>
                <Messenger />
            </Collapse>
            <Modal title='help' open={helper} onClose={toggleHelper}>
                {HelpMessage}
            </Modal>
            {/*<Drawer open={drawer} />*/}
            <TipButton
                TooltipProp={{ title: open ? '隐藏' : '显示', children: <></> }}
                IconButtonProp={{
                    className: classNames(classes.hideButtonShowing, {
                        [classes.hideButton]: !open,
                    }),
                    onClick: toggleOpen,
                }}>
                <KeyboardArrowUp
                    className={classNames({
                        [classes.hide]: !open,
                    })}
                />
                <KeyboardArrowDown
                    className={classNames({
                        [classes.hide]: open,
                    })}
                />
            </TipButton>
        </>
    );
});

export default Bar;
