inline Font CreateUniversalJPFont(int fontSize = 30)
{
    // よく使う日本語文字列
    const char *jpCharset =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
    "あいうえおかきくけこさしすせそたちつてとなにぬねの"
    "はひふへほまみむめもやゆよらりるれろわをん"
    "アィイゥウエォオカキクケコサシスセソタチツテトナニヌネノ"
    "ハヒフヘホマミムメモヤユヨラリルレロワヲン"
    "がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ"
    "ぁぃぅぇぉゃゅょっー。、！？「」()[]{}<>:;+-*/= "
    "漢字"; // サンプルで数個漢字を追加（必要に応じて追加できます）

    int charsCount = 0;
    int *codepoints = LoadCodepoints(jpCharset, &charsCount);

    Font font = LoadFontFromMemory(
        ".ttf",
        DotGothic16_Regular_ttf,
        DotGothic16_Regular_ttf_len,
        fontSize,
        codepoints,
        charsCount
    );

    UnloadCodepoints(codepoints);

    return font;
}
