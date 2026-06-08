void str_rev(char *str)
{
    // int i = 0, l = strlen(str) - 1;

    for (int i = 0, j = strlen(str) - 1; i < j; ++i, --j)
    {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

char *pid_to_ascii(pid_t value, char *buffer, int base)
{
    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    pid_t i = 0;

    switch (base)
    {
    case 2:
    {
        while (value > 0)
        {
            buffer[i++] = (value & 1) ? '1' : '0';
            value >>= 1;
        }
        // buffer[i] = '\0';
        strcpy(buffer + i, "b0");
        str_rev(buffer);
        break;
    }

    case 8:
    {
        while (value > 0)
        {
            buffer[i++] = '0' + (value & 7);
            value >>= 3;
        }
        // buffer[i] = '\0';
        strcpy(buffer + i, "o0");
        str_rev(buffer);
        break;
    }

    case 16:
    {
        while (value > 0)
        {
            signed char digit = value & 15; // 1111
            buffer[i++] = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
            value >>= 4;
        }
        // buffer[i] = '\0';
        strcpy(buffer + i, "x0");
        str_rev(buffer);
        break;
    }

    default:
    {
        while (value > 0)
        {
            pid_t digit = value % base;
            value /= base;
            buffer[i++] = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
        }
        buffer[i] = '\0';
        str_rev(buffer);
        break;
    }
    }

    return buffer;
}
