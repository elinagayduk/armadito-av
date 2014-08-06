#include <libumwsu/module.h>

#include <assert.h>
#include <clamav.h>
#include <stdlib.h>
#include <stdio.h>

const char *clamav_mime_types[] = {
  "application/x-dosexec",
  NULL,
};

struct clamav_data {
  struct cl_engine *clamav_engine;
};

enum umw_status clamav_scan(const char *path, void *mod_data)
{
  struct clamav_data *cl_data = (struct clamav_data *)mod_data;
  const char *virus_name = NULL;
  long unsigned int scanned = 0;
  int cl_scan_status;

  cl_scan_status = cl_scanfile(path, &virus_name, &scanned, cl_data->clamav_engine, CL_SCAN_STDOPT);

  if (cl_scan_status == CL_VIRUS) {
    fprintf(stderr, "%s is infected by virus %s!\n", path, virus_name);
    return UMW_MALWARE;
  } else {
    fprintf(stderr, "%s is not infected by a virus!\n", path);
  }

  return UMW_CLEAN;
}

enum umw_mod_status clamav_init(void **pmod_data)
{
  const char *clamav_db_dir;
  unsigned int signature_count = 0;
  struct clamav_data *cl_data;

  cl_data = (struct clamav_data *)malloc(sizeof(struct clamav_data));
  assert(cl_data != NULL);

  *pmod_data = cl_data;

  cl_data->clamav_engine = cl_engine_new();

  clamav_db_dir = cl_retdbdir();

  if (cl_load(clamav_db_dir, cl_data->clamav_engine, &signature_count, CL_DB_STDOPT) != CL_SUCCESS)
    return UMW_MOD_INIT_ERROR;

  fprintf(stderr, "ClamAV database loaded from %s, %d signatures\n", clamav_db_dir, signature_count);

  if (cl_engine_compile(cl_data->clamav_engine) != CL_SUCCESS)
    return UMW_MOD_INIT_ERROR;

  fprintf(stderr, "ClamAV is initialized\n");

  return UMW_MOD_OK;
}

void clamav_install(void)
{
  if (cl_init(CL_INIT_DEFAULT) != CL_SUCCESS) {
    fprintf(stderr, "ClamAV initialization failed\n");
    return;
  }

  fprintf(stderr, "ClamAV module installed\n");
}
