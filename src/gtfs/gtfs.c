#include <stdlib.h>
#include <stdio.h>

#include <mz.h>
#include <mz_zip.h>
#include <mz_strm_mem.h>
#include <wchar.h>
#include <uriparser/Uri.h>
//#include <curl>

#include "debug.h"

#include "gtfs.h"

gtfs_schedule_t *gtfs_load_schedule(wchar_t *uri_string)
{
    UriUriW uri;
    if (uriParseSingleUriW(&uri, uri_string, NULL) != URI_SUCCESS)
    {
        DEBUG_ERROR("Unable to parse URI: %ls", uri_string);
        goto error;
    }

    //strea

    FILE *fp;
    char *file_stream = NULL;
    void *stream = NULL;

    if (!wcsncmp(uri.scheme.first, L"file", (size_t) (uri.scheme.afterLast - uri.scheme.first)))
    {
        wchar_t *file = malloc(sizeof(wchar_t) * wcslen(uri_string) + 1);

        uriUriStringToUnixFilenameW(uri_string, file);

        char *sfile = malloc(sizeof(char) * wcslen(file) + 1);
        //memset( sfile, 0, sizeof(char) * wcslen(file) + 1);
        wcstombs(sfile, file, sizeof(char) * wcslen(file) + 1);
        size_t file_len = 0;
        fp = fopen(sfile + 1, "r");

        if (!fp)
        {
            DEBUG_ERROR("Unable to load file: %ls", file);
            free(file);
            free(sfile);
            goto error;
        }

        if (fseek(fp, 0L, SEEK_END) == 0)
        {
            file_len = ftell(fp);
            if (file_len == -1) {  }

            file_stream = malloc(file_len);

            if (fseek(fp, 0L, SEEK_SET) != 0)
            {
                DEBUG_ERROR("TODO IDK WHAT ERROR THIS IS"); //TODO: FIX
            }

            size_t newLen = fread(file_stream, sizeof(char), file_len, fp);
            if (ferror( fp ))
            {
                DEBUG_ERROR("Unable to read file: %ls", file);
                fclose(fp);
                free(file_stream);
                goto error;
            } else {
                file_stream[newLen++] = '\0';
            }
        }

        stream = mz_stream_mem_create();
        mz_stream_mem_set_buffer(stream, file_stream, (int32_t)file_len);
        if (mz_stream_mem_open(stream, NULL, MZ_OPEN_MODE_READ))
        {
            DEBUG_ERROR("Unable to load file: %ls", file);
            free(file);
            free(sfile);
            goto error;
        }

        fclose(fp);
        free(file);
        free(sfile);
    } else if (!wcsncmp(uri.scheme.first, L"http", (size_t) (uri.scheme.afterLast - uri.scheme.first))
            || !wcsncmp(uri.scheme.first, L"https", (size_t) (uri.scheme.afterLast - uri.scheme.first)))
    {
        DEBUG_ERROR("HTTP[S] not implemented yet");
        goto error;
    } else
    {
        DEBUG_ERROR("Invalid URI Scheme: %ls", uri_string);
        goto error;
    }

    void *zip_handle = mz_zip_create();

    if (mz_zip_open(zip_handle, stream, MZ_OPEN_MODE_READ))
    {
        DEBUG_ERROR("Zip file corrupt");
        goto error;
    }

    int32_t err = mz_zip_goto_first_entry(zip_handle);
    if (err == MZ_OK) {
        mz_zip_file *file_info = NULL;
        err = mz_zip_entry_get_info(zip_handle, &file_info);
        if (err == MZ_OK) {
            printf("First entry is %s\n", file_info->filename);
        }
    }


    //TODO: MUST USE _WFOPEN
error:

    free(file_stream);

    mz_stream_mem_delete(&stream);
    mz_zip_close(zip_handle);
    mz_zip_delete(&zip_handle);
    uriFreeUriMembersW(&uri);

    return NULL;
}