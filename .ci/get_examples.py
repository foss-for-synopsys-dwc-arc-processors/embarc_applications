import os
import json
import argparse
import git
from embarc_tools.utils import cd, getcwd
from embarc_tools.settings import MAKEFILENAMES


def parse_args():
    parser = argparse.ArgumentParser(
                description="Generate a sanitycheck argument for for tests "
                            " that have changed")
    parser.add_argument("--root", default=getcwd(),
            help="Specify the repo root directory")
    parser.add_argument('--commit_start', default=None,
            help="Commit range start point")
    parser.add_argument('--commit_end', default=None,
            help="Commit range end point")
    parser.add_argument('--only_failed', action="store_true",
            help="Commit range end point")
    return parser.parse_args()


def find_embarc_appl(root):
    cur_path = root
    cur_dir = os.path.dirname(root)
    while cur_path != cur_dir:
        for item in os.listdir(cur_dir):
            if item in MAKEFILENAMES:
                return cur_dir
        cur_path = cur_dir
        cur_dir = os.path.dirname(cur_path)


def get_modified_examples(root, tree0, tree1):
    root = os.path.abspath(root)
    dirname = os.path.basename(root)
    repo = git.Repo(root)
    diff_cmd = ["--name-only"]
    diff_cmd.append("{}..{}".format(tree0, tree1))
    examples_changed = list()
    update = repo.git.diff(diff_cmd).split("\n")
    for item in update:
        if not item.startswith("."):
            item_path = os.path.join(root, item)
            example_root = find_embarc_appl(item_path)
            if example_root:
                example_root = example_root.replace(root, dirname)
                examples_changed.append(example_root)
    return examples_changed


def get_last_failed(file=None):
    if file:
        job = file
    else:
        job = os.environ.get("CI_JOB_NAME")
        if not job:
            job = os.environ.get("NAME")
        if not job:
            job = os.environ.get("STAGE_NAME")
    if job:
        home = os.environ.get("HOME")
        cache = "{}/.cache/result".format(home)
        last_failed_cache = os.path.join(cache, job, "failed_results.json")
        if os.path.exists(last_failed_cache):
            last_failed_appls = json.load(last_failed_cache).keys()
            return last_failed_appls

def main():
    args = parse_args()
    if args.commit_start and args.commit_end:
        examples_changed = get_modified_examples(args.root, 
            args.commit_start, args.commit_end)
        if examples_changed:
            print("%s" %(",".join(examples_changed)))
    if args.only_failed:
        last_failed_appls = get_last_failed()
        if last_failed_appls:
            print("%s" %(",".join(last_failed_appls)))

if __name__ == "__main__":
    main()