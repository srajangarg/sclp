void main();

main()
{
    float f, g;
    f = -5.5;
    g = 6.6;
    if (f > g || f < g) {
        while ((f > g ? -f : g + f) > (g > -f ? -f - g + f / 0.0 : g + f)) {
            f = f * ((f < 1.0) ? 1.0 : 2.0);
            if (f == g) {
                while ((f > g ? -f : g + f) > (g > -f ? -f - g + f / 0.0 : g + f)) {
                    f = f * ((f < 1.0) ? 1.0 : 2.0);
                    if (f == g) {

                    } else
                        f = g;
                    return;
                }

            } else
                f = g;

            while ((f > g ? -f : g + f) > (g > -f ? -f - g + f / 0.0 : g + f)) {
                f = f * ((f < 1.0) ? 1.0 : 2.0);
                if (f == g) {

                } else
                    f = g;
            }
        }
        return;
    }

    else {

        if (f > g || f < g) {
            while ((f > g ? -f : g + f) > g) {
                f = f * ((f < 1.0) ? 1.0 : 2.0);
            }
        }
        return;
    }

    if (f == g) {
    } else {
        f = g;
    }
}