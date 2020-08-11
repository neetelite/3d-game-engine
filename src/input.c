bool
key_press(u32 k)
{
	/* TODO(neet): Make it so you can assign a currently using keyboard so you
	   don't have to pass the OsKeyboard to this function */
	struct OsKeyboard *keyboard = &os_input.keyboards[0];

	if(keyboard->keys[k] && !keyboard->down[k])
	{
		return(1);
	}

	return(0);
}

bool
key_release(u32 k)
{
	/* TODO(neet): Make it so you can assign a currently using keyboard so you
	   don't have to pass the OsKeyboard to this function */
	struct OsKeyboard *keyboard = &os_input.keyboards[0];

	if(!keyboard->keys[k] && keyboard->down[k])
	{
		return(1);
	}

	return(0);
}

bool
key_down(u32 k)
{
	/* TODO(neet): Make it so you can assign a currently using keyboard so you
	   don't have to pass the OsKeyboard to this function */
	struct OsKeyboard *keyboard = &os_input.keyboards[0];

	if(keyboard->keys[k])
	{
		return(1);
	}

	return(0);
}

bool
key_up(u32 k)
{
	/* TODO(neet): Make it so you can assign a currently using keyboard so you
	   don't have to pass the OsKeyboard to this function */
	struct OsKeyboard *keyboard = &os_input.keyboards[0];

	if(!keyboard->keys[k])
	{
		return(1);
	}

	return(0);
}

bool
button_press(u32 b)
{
	/* TODO(neet): Make it so you can assign a currently using mouse so you
	   don't have to pass the OsMouse to this function */
	struct OsMouse *mouse = &os_input.mice[0];

	if(mouse->buttons[b] && !mouse->down[b])
	{
		return(1);
	}

	return(0);
}

bool
button_release(u32 b)
{
	/* TODO(neet): Make it so you can assign a currently using mouse so you
	   don't have to pass the Appmouse to this function */
	struct OsMouse *mouse = &os_input.mice[0];

	if(!mouse->buttons[b] && mouse->down[b])
	{
		return(1);
	}

	return(0);
}

bool
button_down(u32 b)
{
	/* TODO(neet): Make it so you can assign a currently using mouse so you
	   don't have to pass the OsMouse to this function */
	struct OsMouse *mouse = &os_input.mice[0];

	if(mouse->buttons[b])
	{
		return(1);
	}

	return(0);
}

bool
button_up(u32 b)
{
	/* TODO(neet): Make it so you can assign a currently using mouse so you
	   don't have to pass the OsMouse to this function */
	struct OsMouse *mouse = &os_input.mice[0];

	if(!mouse->buttons[b])
	{
		return(1);
	}

	return(0);
}
