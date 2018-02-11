def sha256_file(file_path, chunk_size=65336):
    """
    Get the sha 256 checksum of a file.
    :param file_path: path to file
    :type file_path: unicode or str
    :param chunk_size: number of bytes to read in each iteration. Must be > 0.
    :type chunk_size: int
    :return: sha 256 checksum of file
    :rtype : str
    """
    # Read the file in small pieces, so as to prevent failures to read particularly large files.
    # Also ensures memory usage is kept to a minimum. Testing shows default is a pretty good size.
    assert isinstance(chunk_size, int) and chunk_size > 0
    import hashlib
    import functools
    digest = hashlib.sha256()
    with open(file_path, 'rb') as f:
        [digest.update(chunk) for chunk in iter(functools.partial(f.read, chunk_size), '')]
    return digest.hexdigest()
