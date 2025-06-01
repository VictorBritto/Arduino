create table clima (
  id bigserial primary key,
  temperatura float8,
  umidade float8,
  created_at timestamp with time zone default now()
)